#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <utility>
#include "XMFloatUtil.h"
#include "WrapperFactory.h"

class Graphics;

class Render
{
public:
	struct Mesh
	{
		std::shared_ptr<Shader> shader;
		std::unique_ptr<Buffer> buffer;

		Mesh() :
			shader(nullptr),
			buffer(nullptr)
		{

		}
		Mesh(Mesh&& p_Other) :
			buffer(std::move(p_Other.buffer)),
			shader(p_Other.shader)
		{
		}

		Mesh& operator=(Mesh&& p_Other)
		{
			std::swap(buffer, p_Other.buffer);
			std::swap(shader, p_Other.shader);

			return *this;
		}
		//Mesh& operator=(const LinearAllocator&)
		~Mesh()
		{
			shader = nullptr;
			buffer = nullptr;
		}
	private:
		Mesh(const Mesh&);
	};
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
	std::vector<Render::Mesh> m_MeshList;

public:
	Render(void);
	~Render(void);
	void initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	void shutdown(void);

	void draw(void);

	void updateCamera(Vector3 p_Position, Vector3 p_Forward, Vector3 p_Up);
	void addMesh( Mesh p_Mesh);

private:
	void createConstantBuffers();
	void updateConstantBuffer(void);

	void initializeMatrices(int p_ScreenWidth, int p_ScreenHeight, float p_NearZ, float p_FarZ);
};

