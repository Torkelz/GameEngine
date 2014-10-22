#pragma once
#include <memory>
#include "Buffer.h"
#include "Shader.h"
#include "Utilities.h"
#include "IResourceManager.h"

struct Mesh
{
	std::shared_ptr<Shader> shader;
	std::unique_ptr<Buffer> buffer;
	std::unique_ptr<Buffer> indexBuffer;
	std::vector<ID3D11ShaderResourceView*> diffusemaps;
	std::vector<Res::Material> materials;
	std::vector<int> faceGroups;

	Mesh() :
		shader(nullptr),
		buffer(nullptr)

	{

	}
	Mesh(Mesh&& p_Other) :
		buffer(std::move(p_Other.buffer)),
		shader(p_Other.shader),
		indexBuffer(std::move(p_Other.indexBuffer)),
		diffusemaps(std::move(p_Other.diffusemaps)),
		faceGroups(std::move(p_Other.faceGroups))
	{
	}

	Mesh& operator=(Mesh&& p_Other)
	{
		std::swap(buffer, p_Other.buffer);
		std::swap(shader, p_Other.shader);
		std::swap(indexBuffer, p_Other.indexBuffer);
		std::swap(diffusemaps, p_Other.diffusemaps);
		std::swap(faceGroups, p_Other.faceGroups);

		return *this;
	}
	~Mesh()
	{
		shader = nullptr;
		buffer = nullptr;

		for (ID3D11ShaderResourceView* s : diffusemaps)
		{
			SAFE_RELEASE(s);
		}
	}
	void setSRV(ID3D11ShaderResourceView* p_newSRV, int p_DiffuseIndex)
	{
		std::swap(diffusemaps.at(p_DiffuseIndex), p_newSRV);
		SAFE_RELEASE(p_newSRV);
	}
	
private:
	Mesh(const Mesh&);
};