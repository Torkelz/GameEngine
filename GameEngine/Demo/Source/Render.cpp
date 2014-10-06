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

	
	m_CBCameraFixed->setBuffer(0);
	m_CBCamera->setBuffer(1);
	m_Graphics->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (std::vector<Mesh>::const_iterator &it = m_MeshList.cbegin(); it != m_MeshList.cend(); ++it)
	{
		it->shader->setShader();
		it->buffer->setBuffer(0);
		m_Graphics->getDeviceContext()->Draw(it->buffer->getNumOfElements(), 0);
		it->buffer->unsetBuffer(0);
		it->shader->unSetShader();
	}
	
	

	m_CBCameraFixed->unsetBuffer(0);
	m_CBCamera->unsetBuffer(1);
	

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
	m_ScreenHeight = (float)p_ScreenHeight;
	m_ScreenWidth = (float)p_ScreenWidth;
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

void Render::addMesh( Mesh p_Mesh)
{
	m_MeshList.push_back(std::move(p_Mesh));
}
