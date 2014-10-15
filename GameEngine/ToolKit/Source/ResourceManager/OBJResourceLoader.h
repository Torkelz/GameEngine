#pragma once
#include "IResourceLoader.h"
#include "OBJResourceExtraData.h"

#include <DirectXMath.h>

namespace Res
{
	class OBJResourceLoader : public IResourceLoader
	{
	public:
		typedef unsigned int UINT;

		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 texCoords;
			Vertex(DirectX::XMFLOAT3 p_Pos, DirectX::XMFLOAT3 p_Normal, DirectX::XMFLOAT2 p_TexCoords) : pos(p_Pos), normal(p_Normal), texCoords(p_TexCoords)
			{}
			Vertex(){}
		};

	public:
		virtual std::string getPattern(void);
		virtual bool useRawFile(void);
		virtual bool discardRawBufferAfterLoad(void);
		virtual UINT getLoadedResourceSize(char *p_RawBuffer, UINT p_RawSize);
		virtual bool loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle);
	private:
		bool parseOBJ(char *p_ObjStream, size_t p_Length, std::shared_ptr<ResourceHandle> p_Handle);
	};
}

