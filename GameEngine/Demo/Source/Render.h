#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "XMFloatUtil.h"
#include "WrapperFactory.h"

class Graphics;

class Render
{
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

	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
	};
	Buffer *temporarybox;
	Shader *temporaryShader;

public:
	Render(void);
	~Render(void);
	void initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	void shutdown(void);

	void draw(void);

	void updateCamera(Vector3 p_Position, Vector3 p_Forward, Vector3 p_Up);

private:
	void createConstantBuffers();
	void updateConstantBuffer(void);

	void initializeMatrices(int p_ScreenWidth, int p_ScreenHeight, float p_NearZ, float p_FarZ);

	Vertex* createBox(int size, DirectX::XMVECTOR center);

	inline Vertex createVertex(DirectX::XMVECTOR _position, DirectX::XMVECTOR _normal)
	{
		Vertex v;

		DirectX::XMStoreFloat3(&v.position, _position);
		DirectX::XMStoreFloat3(&v.normal, _normal);

		return v;
	}
};

