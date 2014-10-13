#pragma once
#include "IResourceLoader.h"
#include "OBJResourceExtraData.h"

namespace Res
{
	class OBJResourceLoader : public IResourceLoader
	{
	public:
		typedef unsigned int UINT;
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

