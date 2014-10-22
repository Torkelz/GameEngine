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
class Res::ResourceManager;


class Render
{
public:
	typedef unsigned int UINT;
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
	Buffer *m_CBWorld;

	UINT m_NextModelInstanceID;
	std::map<std::string, Mesh> m_MeshList;
	std::map<UINT, MeshInstance> m_MeshInstanceList;
	std::vector<UINT> m_RenderList;

	Res::ResourceManager *m_ResourceManager;

	ID3D11SamplerState *m_SamplerState;

	struct cbCamera
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4 eyePos;
	};

public:
	Render(void);
	~Render(void);
	void initialize(HWND p_Hwnd, Res::ResourceManager *p_ResourceManager, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	void shutdown(void);

	void draw(void);

	void drawMeshInstance(UINT p_Instance);

	void updateCamera(Vector3 p_Position, Vector3 p_Forward, Vector3 p_Up);
	void createMesh(std::weak_ptr<Res::ResourceHandle> p_ResourceHandle);
	int	 createMeshInstance(const std::string p_MeshName);
	MeshInstance *getMeshInstance(UINT p_InstanceId);
	void changeTexture(std::string p_MeshName, int p_DiffuseIndex, std::shared_ptr<Res::ResourceHandle> p_TextureHandle);

	Graphics *getGraphics();

	void begin(void);
	void end(void);

	void setCameraBuffers();
	void unsetCameraBuffers();

private:
	void createConstantBuffers();
	void createSamplerState();
	void updateConstantBuffer(Vector3 p_Position);

	void initializeMatrices(int p_ScreenWidth, int p_ScreenHeight, float p_NearZ, float p_FarZ);
};

