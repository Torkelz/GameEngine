#pragma once
#include <memory>
#include "Buffer.h"
#include "Shader.h"
#include "IResourceManager.h"

struct Mesh
{
	std::shared_ptr<Shader> shader;
	std::unique_ptr<Buffer> buffer;
	std::unique_ptr<Buffer> indexBuffer;
	std::vector<ID3D11ShaderResourceView*> diffusemaps;
	std::vector<Res::Material> materials;

	Mesh() :
		shader(nullptr),
		buffer(nullptr),
		indexBuffer(nullptr)

	{

	}
	Mesh(Mesh&& p_Other) :
		buffer(std::move(p_Other.buffer)),
		shader(p_Other.shader),
		indexBuffer(std::move(p_Other.indexBuffer))
	{
	}

	Mesh& operator=(Mesh&& p_Other)
	{
		std::swap(buffer, p_Other.buffer);
		std::swap(shader, p_Other.shader);
		std::swap(indexBuffer, p_Other.indexBuffer);

		return *this;
	}
	~Mesh()
	{
		shader = nullptr;
		buffer = nullptr;
		indexBuffer = nullptr;
	}
private:
	Mesh(const Mesh&);
};