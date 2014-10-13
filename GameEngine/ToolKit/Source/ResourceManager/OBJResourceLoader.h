#pragma once
#include "IResourceLoader.h"
#include "OBJResourceExtraData.h"

namespace Res
{
	class OBJResourceLoader : public IResourceLoader
	{
	public:
		typedef unsigned int UINT;

		struct Vec3
		{
			float x, y, z;
			Vec3(float p_X, float p_Y, float p_Z) : x(p_X), y(p_Y), z(p_Z)
			{}
			Vec3(){}
		};
		struct Vec2
		{
			float x, y;
			Vec2(float p_X, float p_Y) : x(p_X), y(p_Y)
			{}
			Vec2(){}
		};

		struct Vertex
		{
			Vec3 pos, normal;
			Vec2 texCoords;
			Vertex(Vec3 p_Pos, Vec3 p_Normal, Vec2 p_TexCoords) : pos(p_Pos), normal(p_Normal), texCoords(p_TexCoords)
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

