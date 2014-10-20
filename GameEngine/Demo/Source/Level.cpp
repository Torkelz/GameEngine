#include "Level.h"
#include "ModifyMesh.h"

Level::Level() :
	m_Render(nullptr)
{
}


Level::~Level()
{
}

void Level::initialize(Render *p_Render)
{
	m_Render = p_Render;
	Buffer::Description bDesc = {};
	bDesc.initData = createBox(10, DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f));
	bDesc.numOfElements = 36;
	bDesc.sizeOfElement = sizeof(Vertex);
	bDesc.type = Buffer::Type::VERTEX_BUFFER;
	bDesc.usage = Buffer::Usage::DEFAULT;


	using namespace Res;

	ResourceZipFile zip = ResourceZipFile();
	zip.initialize(L"..\\Resources\\Optimus.zip");

	ResourceManager man(1000000000);

	man.init();
	man.loadResource(&zip, "hubba3");

	man.registerLoader(std::shared_ptr<IResourceLoader>(new OBJResourceLoader()));

	Resource re("hubba\\models\\optimus.obj", "hubba3");
	//Resource re("Optimus\\VH-Optimus.obj", "hubba3");

	std::weak_ptr<ResourceHandle> texture = man.getHandle(&re);


	m_Render->createMesh(texture);

	lamp = m_Render->createMeshInstance(texture.lock()->getName());
	ModifyMesh::setMeshPosition(lamp, Vector3(0, 0, 0));
	ModifyMesh::setMeshScale(lamp, Vector3(1, 1, 1));
	//ModifyMesh::setMeshRotation(lamp, Vector3(0, -45, 0));
}

void Level::update()
{

}

void Level::draw()
{
	m_Render->drawMeshInstance(lamp);
}

Level::Vertex* Level::createBox(int size, DirectX::XMVECTOR center)
{
	using namespace DirectX;

	Vertex* box = new Vertex[36];
	XMVECTOR normal;
	XMVECTOR vert0 = center + XMVectorSet(-1.0f*size, -1.0f*size, -1.0f*size, 1); // 0 --- LowerLeftFront
	XMVECTOR vert1 = center + XMVectorSet(1.0f*size, -1.0f*size, -1.0f*size, 1); // 1 +-- LowerRightFront
	XMVECTOR vert2 = center + XMVectorSet(-1.0f*size, 1.0f*size, -1.0f*size, 1); // 2 -+- UpperLeftFront
	XMVECTOR vert3 = center + XMVectorSet(1.0f*size, 1.0f*size, -1.0f*size, 1); // 3 ++- UpperRightFront
	XMVECTOR vert4 = center + XMVectorSet(-1.0f*size, -1.0f*size, 1.0f*size, 1); // 4 --+ LowerLeftBack
	XMVECTOR vert5 = center + XMVectorSet(1.0f*size, -1.0f*size, 1.0f*size, 1); // 5 +-+ LowerRightBack
	XMVECTOR vert6 = center + XMVectorSet(-1.0f*size, 1.0f*size, 1.0f*size, 1); // 6 -++ UpperLeftBack
	XMVECTOR vert7 = center + XMVectorSet(1.0f*size, 1.0f*size, 1.0f*size, 1); // 7 +++ UpperRightBack
	// Back
	normal = XMVectorSet(0, 0, 1, 0);
	box[0] = createVertex(vert4, normal);
	box[1] = createVertex(vert6, normal);
	box[2] = createVertex(vert5, normal);
	box[3] = createVertex(vert6, normal);
	box[4] = createVertex(vert7, normal);
	box[5] = createVertex(vert5, normal);

	// Front
	normal = XMVectorSet(0, 0, -1, 0);

	box[6] = createVertex(vert1, normal);
	box[7] = createVertex(vert3, normal);
	box[8] = createVertex(vert0, normal);
	box[9] = createVertex(vert3, normal);
	box[10] = createVertex(vert2, normal);
	box[11] = createVertex(vert0, normal);

	// Top
	normal = XMVectorSet(0, 1, 0, 0);
	box[12] = createVertex(vert3, normal);
	box[13] = createVertex(vert7, normal);
	box[14] = createVertex(vert2, normal);
	box[15] = createVertex(vert7, normal);
	box[16] = createVertex(vert6, normal);
	box[17] = createVertex(vert2, normal);

	// Bottom
	normal = XMVectorSet(0, -1, 0, 0);
	box[18] = createVertex(vert0, normal);
	box[19] = createVertex(vert4, normal);
	box[20] = createVertex(vert1, normal);
	box[21] = createVertex(vert4, normal);
	box[22] = createVertex(vert5, normal);
	box[23] = createVertex(vert1, normal);

	// Right						
	normal = XMVectorSet(1, 0, 0, 0);
	box[24] = createVertex(vert5, normal);
	box[25] = createVertex(vert7, normal);
	box[26] = createVertex(vert1, normal);
	box[27] = createVertex(vert7, normal);
	box[28] = createVertex(vert3, normal);
	box[29] = createVertex(vert1, normal);

	// Left
	normal = XMVectorSet(-1, 0, 0, 0);
	box[30] = createVertex(vert0, normal);
	box[31] = createVertex(vert2, normal);
	box[32] = createVertex(vert4, normal);
	box[33] = createVertex(vert2, normal);
	box[34] = createVertex(vert6, normal);
	box[35] = createVertex(vert4, normal);
	return box;
}
