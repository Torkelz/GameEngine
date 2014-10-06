#include "Render.h"
#include "Graphics.h"
#include "Utilities.h"

Render::Render(void) :
	m_Graphics(nullptr),
	m_CBCameraFixed(nullptr),
	m_CBCamera(nullptr)
{
}


Render::~Render(void)
{
}

void Render::initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen)
{
	if (m_Graphics)
	{
		m_Graphics->shutdown();
		SAFE_DELETE(m_Graphics);
	}
	m_Graphics = new Graphics();
	m_Graphics->initialize(p_Hwnd, p_ScreenWidth, p_ScreenHeight, p_Fullscreen);

	float nearZ = 8.0f;
	float farZ = 100000.0f;
	m_FOV = 2 * 3.1415f / 360.0f * 45; //45 degrees
	initializeMatrices(p_ScreenWidth, p_ScreenHeight, nearZ, farZ);

	createConstantBuffers();
}

void Render::shutdown(void)
{
	if (m_Graphics)
	{
		m_Graphics->shutdown();
		SAFE_DELETE(m_Graphics);
	}
}

void Render::draw(void)
{
	static float normal[] = { 0.5f, 0.5f, 0.5f, 1.f };
	m_Graphics->begin(normal);


	m_Graphics->end();
}

void Render::updateCamera(Vector3 p_Position, Vector3 p_Forward, Vector3 p_Up)
{
	using namespace DirectX;

	m_Eye = p_Position;
	XMVECTOR upVec = XMVectorSet(p_Up.x, p_Up.y, p_Up.z, 0.f);
	XMVECTOR forwardVec = XMVectorSet(p_Forward.x, p_Forward.y, p_Forward.z, 0.f);
	XMVECTOR pos = XMVectorSet(p_Position.x, p_Position.y, p_Position.z, 1.f);
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixLookToLH(pos, forwardVec, upVec)));
	updateConstantBuffer();
}

void Render::createConstantBuffers()
{
	Buffer::Description bDesc = {};
	bDesc.initData = &m_ProjectionMatrix;
	bDesc.numOfElements = 1;
	bDesc.sizeOfElement = sizeof(DirectX::XMFLOAT4X4);
	bDesc.type = Buffer::Type::CONSTANT_BUFFER_ALL;
	bDesc.usage = Buffer::Usage::USAGE_IMMUTABLE;

	m_CBCameraFixed = WrapperFactory::getInstance()->createBuffer(bDesc);

	bDesc.initData = &m_ViewMatrix;
	bDesc.usage = Buffer::Usage::DEFAULT;

	m_CBCamera = WrapperFactory::getInstance()->createBuffer(bDesc);
}

void Render::updateConstantBuffer(void)
{
	m_Graphics->getDeviceContext()->UpdateSubresource(m_CBCamera->getBufferPointer(), NULL, NULL, &m_ViewMatrix, sizeof(DirectX::XMFLOAT4X4), NULL);
}


void Render::initializeMatrices(int p_ScreenWidth, int p_ScreenHeight, float p_NearZ, float p_FarZ)
{
	using namespace DirectX;
	Vector4 eye;
	Vector4 lookAt;
	Vector4 up;
	m_Eye = Vector3(0, 0, -20);
	m_FarZ = p_FarZ;
	m_NearZ = p_NearZ;
	m_ScreenHeight = p_ScreenHeight;
	m_ScreenWidth = p_ScreenWidth;
	eye = Vector4(m_Eye.x, m_Eye.y, m_Eye.z, 1);
	lookAt = Vector4(0, 0, 0, 1);
	up = Vector4(0, 1, 0, 0);
	m_ViewMatrix = XMFLOAT4X4();
	m_ProjectionMatrix = XMFLOAT4X4();
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixLookAtLH(XMLoadFloat4(&eye),
		XMLoadFloat4(&lookAt), XMLoadFloat4(&up))));
	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixTranspose(XMMatrixPerspectiveFovLH(m_FOV,
		(float)p_ScreenWidth / (float)p_ScreenHeight, p_NearZ, m_FarZ)));
}

Render::Vertex* Render::CreateBox(int size, DirectX::XMVECTOR center)
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
	box[0] = CreateVertex(vert4, normal);
	box[1] = CreateVertex(vert6, normal);
	box[2] = CreateVertex(vert5, normal);
	box[3] = CreateVertex(vert6, normal);
	box[4] = CreateVertex(vert7, normal);
	box[5] = CreateVertex(vert5, normal);

	// Front
	normal = XMVectorSet(0, 0, -1, 0);

	box[6] = CreateVertex(vert1, normal);
	box[7] = CreateVertex(vert3, normal);
	box[8] = CreateVertex(vert0, normal);
	box[9] = CreateVertex(vert3, normal);
	box[10] = CreateVertex(vert2, normal);
	box[11] = CreateVertex(vert0, normal);

	// Top
	normal = XMVectorSet(0, 1, 0, 0);
	box[12] = CreateVertex(vert3, normal);
	box[13] = CreateVertex(vert7, normal);
	box[14] = CreateVertex(vert2, normal);
	box[15] = CreateVertex(vert7, normal);
	box[16] = CreateVertex(vert6, normal);
	box[17] = CreateVertex(vert2, normal);

	// Bottom
	normal = XMVectorSet(0, -1, 0, 0);
	box[18] = CreateVertex(vert0, normal);
	box[19] = CreateVertex(vert4, normal);
	box[20] = CreateVertex(vert1, normal);
	box[21] = CreateVertex(vert4, normal);
	box[22] = CreateVertex(vert5, normal);
	box[23] = CreateVertex(vert1, normal);

	// Right						
	normal = XMVectorSet(1, 0, 0, 0);
	box[24] = CreateVertex(vert5, normal);
	box[25] = CreateVertex(vert7, normal);
	box[26] = CreateVertex(vert1, normal);
	box[27] = CreateVertex(vert7, normal);
	box[28] = CreateVertex(vert3, normal);
	box[29] = CreateVertex(vert1, normal);

	// Left
	normal = XMVectorSet(-1, 0, 0, 0);
	box[30] = CreateVertex(vert0, normal);
	box[31] = CreateVertex(vert2, normal);
	box[32] = CreateVertex(vert4, normal);
	box[33] = CreateVertex(vert2, normal);
	box[34] = CreateVertex(vert6, normal);
	box[35] = CreateVertex(vert4, normal);
	return box;
}