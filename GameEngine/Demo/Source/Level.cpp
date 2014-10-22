#include "Level.h"
#include "ModifyMesh.h"

Level::Level() :
	m_Render(nullptr)
{
}


Level::~Level()
{
}

void Level::initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager)
{
	m_Render = p_Render;
	m_ResourceManager = p_ResourceManager;

	using namespace Res;
	ResourceZipFile zip = ResourceZipFile();
	zip.initialize(L"..\\Resources\\hubba3.zip");
	
	p_ResourceManager->loadZipLib(&zip, "hubba3");	
	
	Resource re("hubba\\models\\optimus.obj", "hubba3");
	std::weak_ptr<ResourceHandle> model = m_ResourceManager->getHandle(&re);	

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
}

void Level::draw()
{
	m_Render->drawMeshInstance(lamp);
	m_Particles.render();
}
