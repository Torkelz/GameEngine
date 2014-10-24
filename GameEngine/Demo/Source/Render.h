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

	/**
	* Initializes the rendering relevant components.
	* @param p_Hwnd, a handle to a window.
	* @param p_ResourceManager, a pointer to a resource manager. Cannot be nullptr.
	* @param p_ScreenWidth, the windows width.
	* @param p_ScreenHeight, the windows height.
	* @param p_Fullscreen, true if it should be in fullscreen.
	*/
	void initialize(HWND p_Hwnd, Res::ResourceManager *p_ResourceManager, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	
	/**
	* Cleans up all rendering components.
	*/
	void shutdown(void);

	/**
	* Draws all mesh instances.
	*/
	void draw(void);

	/**
	* Queue the mesh instance to be drawn at the next draw call.
	* @param p_Instance, the mesh instance identifier.
	*/
	void drawMeshInstance(UINT p_Instance);

	/**
	* Updates the camera variables.
	* @param p_Position, the position of the camera in world coordinates.
	* @param p_Forward, the direction of the camera should be facing.
	* @param p_Up, the cameras up direction.
	*/
	void updateCamera(Vector3 p_Position, Vector3 p_Forward, Vector3 p_Up);

	/**
	* Creates a mesh from a resourceHandle.
	* @param p_ResourceHandle, the resourceHandle containing all the relevant information.
	*/
	void createMesh(std::weak_ptr<Res::ResourceHandle> p_ResourceHandle);

	/**
	* Creates a mesh instance from specified mesh name.
	* @param p_MeshName, name of the mesh.
	* @return, returns unique identifier for the mesh instance.
	*/
	int	 createMeshInstance(const std::string p_MeshName);

	/**
	* Get Mesh instance information.
	* @param p_InstanceId, identifier for the mesh instance.
	* @return, pointer to mesh instance.
	*/
	MeshInstance *getMeshInstance(UINT p_InstanceId);

	/**
	* Changes a texture of a mesh.
	* @param p_MeshName, the meshe name.
	* @param p_DiffuseIndex, the diffuse texture index to be replaced.
	* @param p_TextureHandle, the texture handle to the new texture.
	*/
	void changeTexture(std::string p_MeshName, int p_DiffuseIndex, std::shared_ptr<Res::ResourceHandle> p_TextureHandle);
	
	/**
	* Get a pointer to the graphics component.
	* @return, pointer to graphics. Can be nullptr.
	*/
	Graphics *getGraphics(void);

	/**
	* Calls Graphic's begin function.
	*/
	void begin(void);

	/**
	* Calls Graphic's end function.
	*/
	void end(void);

	/**
	* Sets cameras constant buffers
	*/
	void setCameraBuffers(void);

	/**
	* Unsets cameras constant buffers
	*/
	void unsetCameraBuffers(void);
private:
	void createConstantBuffers(void);
	void createSamplerState(void);
	void updateConstantBuffer(Vector3 p_Position);

	void initializeMatrices(int p_ScreenWidth, int p_ScreenHeight, float p_NearZ, float p_FarZ);
};

