#include "Level.h"
#include "ModifyMesh.h"

Level::Level() :
	m_Render(nullptr)
{
}


Level::~Level()
{
}

void Level::initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager, Vector3 *p_CameraPos)
{
	green = false;
	red = false;
	blue = true;
	m_Render = p_Render;
	m_ResourceManager = p_ResourceManager;
	m_CameraPos = p_CameraPos;
	using namespace Res;
	m_OptimusRed = Resource("hubba\\models\\optimusred.bmp", "hubba3");
	m_OptimusGreen = Resource("hubba\\models\\optimusgreen.bmp", "hubba3");
	m_OptimusObj = Resource("hubba\\models\\optimus.obj", "hubba3");
	m_OptimusBlue = Resource("hubba\\models\\optimusp.bmp", "hubba3");


	m_zip.initialize(L"..\\Resources\\hubba3.zip");
	
	m_ResourceManager->loadZipLib(&m_zip, "hubba3");
	
	
	std::weak_ptr<ResourceHandle> model = m_ResourceManager->getHandle(&m_OptimusObj);

	m_Render->createMesh(model);

	lamp = m_Render->createMeshInstance(model.lock()->getName());
	ModifyMesh::setMeshPosition(lamp, Vector3(0, 0, 0));
	ModifyMesh::setMeshScale(lamp, Vector3(3, 3, 3));
	ModifyMesh::setMeshRotation(lamp, Vector3(0, -45, 0));

	const unsigned int maxParticles = 1000;
	m_LinAlloc = new Allocator::LinearAllocator(sizeof(Particles::Particle) * maxParticles * 2);

	m_Particles.initialize((char*)m_LinAlloc->allocate(sizeof(Particles::Particle) * maxParticles), maxParticles,
		DirectX::XMFLOAT3(0, 0, 0), 10.f, 0.00001f, m_Render);
}

void Level::update(float p_Dt)
{
	m_Particles.update(p_Dt);
	Vector3 meshPos = m_Render->getMeshInstance(lamp)->position;
	using DirectX::operator-;
	DirectX::XMVECTOR vDist = DirectX::XMLoadFloat3(&meshPos) - DirectX::XMLoadFloat3(m_CameraPos);
	float dist = DirectX::XMVector3Length(vDist).m128_f32[0];

	if (dist < 10 && !blue)
	{
		m_Render->changeTexture("hubba\\models\\optimus.obj", 0, m_ResourceManager->getHandle(&m_OptimusBlue));
		green = false;
		red = false;
		blue = true;
	}
	if (dist > 10 && dist < 20 && !green)
	{

		m_Render->changeTexture("hubba\\models\\optimus.obj", 0, m_ResourceManager->getHandle(&m_OptimusGreen));
		green = true;
		red = false;
		blue = false;
	}

	if (dist > 20 && !red)
	{
		m_Render->changeTexture("hubba\\models\\optimus.obj", 0, m_ResourceManager->getHandle(&m_OptimusRed));
		green = false;
		red = true;
		blue = false;

	}
}

void Level::draw()
{
	m_Render->drawMeshInstance(lamp);
	m_Particles.render();
}
