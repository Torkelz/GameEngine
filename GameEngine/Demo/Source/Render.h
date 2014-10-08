#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <utility>
#include <map>
#include "XMFloatUtil.h"
#include "WrapperFactory.h"
#include "IResourceManager.h"
#include "Mesh.h"
#include "MeshInstance.h"

class Graphics; 


class Render
{
public:

private:
	Graphics *m_Graphics;

	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	Vector3 m_Eye;
	float m_FOV;
	float m_FarZ;
	float m_NearZ;
	float m_ScreenHeight;
	float m_ScreenWidth;

	Buffer *m_CBCameraFixed;
	Buffer *m_CBCamera;
	UINT m_NextModelInstanceID;
	std::map<std::string, Mesh> m_MeshList;
	std::map<UINT, MeshInstance> m_MeshList;

public:
	Render(void);
	~Render(void);
	void initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	void shutdown(void);

	void draw(void);

	void updateCamera(Vector3 p_Position, Vector3 p_Forward, Vector3 p_Up);
	void createMesh(std::weak_ptr<Res::ResourceHandle> p_ResourceHandle);

private:
	void createConstantBuffers();
	void updateConstantBuffer(void);

	void initializeMatrices(int p_ScreenWidth, int p_ScreenHeight, float p_NearZ, float p_FarZ);
};

