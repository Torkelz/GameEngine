#include "OBJResourceLoader.h"

namespace Res
{
	std::string OBJResourceLoader::getPattern(void)
	{
		return "*.obj";
	}

	bool OBJResourceLoader::useRawFile(void)
	{
		return false;
	}

	bool OBJResourceLoader::discardRawBufferAfterLoad(void)
	{
		return true;
	}

	OBJResourceLoader::UINT OBJResourceLoader::getLoadedResourceSize(char *p_RawBuffer, UINT p_RawSize)
	{
		return p_RawSize; //Might be wrong!
	}

	bool OBJResourceLoader::loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle)
	{
		return parseOBJ(p_RawBuffer, p_RawSize, p_Handle);
	}

	bool OBJResourceLoader::parseOBJ(char *p_ObjStream, size_t p_Length, std::shared_ptr<ResourceHandle> p_Handle)
	{
		return true;
	}
}
