#include "Level.h"
#include "ModifyMesh.h"

Level::Level(void) :
	m_Render(nullptr), m_CameraPos(nullptr),
	m_Green(false),	m_Red(false), m_Blue(true),
	m_ResourceManager(nullptr),
	m_LinAlloc(nullptr)
{
}

Level::~Level(void)
{
	m_Render = nullptr;
	m_CameraPos = nullptr;
	m_ResourceManager = nullptr;
	SAFE_DELETE(m_LinAlloc);
}

void Level::initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager, Vector3 *p_CameraPos)
{
	m_Render = p_Render;
	m_ResourceManager = p_ResourceManager;
	m_CameraPos = p_CameraPos;

	using namespace Res;
	//Create file resources.
	m_OptimusRed = Resource("optimusred.bmp", "hobba");
	m_OptimusGreen = Resource("optimusgreen.bmp", "hobba");
	m_OptimusObj = Resource("optimus.obj", "hobba");
	m_OptimusBlue = Resource("optimusp.bmp", "hobba");
	
	//Create and read the mcap header from specified file.
	m_Mcap.initialize(L"..\\Resources\\hobba.mcap");
	
	//Loading the mcap file into the resource manager.
	m_ResourceManager->loadZipLib(&m_Mcap, "hobba");
	
	//Gets handle for the Optimus obj file.
	std::shared_ptr<ResourceHandle> model = m_ResourceManager->getHandle(&m_OptimusObj);

	m_Render->createMesh(model);

	m_OptimusHandle = m_Render->createMeshInstance(model->getName());

	//Sets MeshInstance properties.
	ModifyMesh::setMeshPosition(m_OptimusHandle, Vector3(0, 0, 0));
	ModifyMesh::setMeshScale(m_OptimusHandle, Vector3(3, 3, 3));
	ModifyMesh::setMeshRotation(m_OptimusHandle, Vector3(0, -45, 0));

	//Particle setup.
	const unsigned int maxParticles = 250;
	m_LinAlloc = new Allocator::LinearAllocator(sizeof(Particles::Particle) * maxParticles * 2);

	m_Particles.initialize((char*)m_LinAlloc->allocate(sizeof(Particles::Particle) * maxParticles), maxParticles,
		DirectX::XMFLOAT3(0, 0, 30), 20.f, 0.001f, m_Render);
}

void Level::update(float p_Dt)
{
	m_Particles.update(p_Dt);

	//Optimus texture change depending on distance.
	Vector3 meshPos = m_Render->getMeshInstance(m_OptimusHandle)->position;
	using DirectX::operator-;
	DirectX::XMVECTOR vDist = DirectX::XMLoadFloat3(&meshPos) - DirectX::XMLoadFloat3(m_CameraPos);
	float dist = DirectX::XMVector3Length(vDist).m128_f32[0];

	if (dist < 10 && !m_Blue)
	{
		m_Render->changeTexture("optimus.obj", 0, m_ResourceManager->getHandle(&m_OptimusBlue));
		m_Green = false;
		m_Red = false;
		m_Blue = true;
	}
	if (dist > 10 && dist < 20 && !m_Green)
	{

		m_Render->changeTexture("optimus.obj", 0, m_ResourceManager->getHandle(&m_OptimusGreen));
		m_Green = true;
		m_Red = false;
		m_Blue = false;
	}
	if (dist > 20 && !m_Red)
	{
		m_Render->changeTexture("optimus.obj", 0, m_ResourceManager->getHandle(&m_OptimusRed));
		m_Green = false;
		m_Red = true;
		m_Blue = false;

	}
}

void Level::draw(void)
{
	m_Render->drawMeshInstance(m_OptimusHandle);
	m_Particles.render();
}
