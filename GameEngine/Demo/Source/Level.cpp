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
}

void Level::update()
{

}

void Level::draw()
{
	m_Render->drawMeshInstance(lamp);
}
