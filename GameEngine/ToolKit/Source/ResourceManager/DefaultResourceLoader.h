#pragma once
#include "IResourceLoader.h"

namespace Res
{
	class DefaultResourceLoader : public IResourceLoader
	{
	public:
		typedef unsigned int UINT;
	public:
		virtual bool useRawFile(void);
		virtual bool discardRawBufferAfterLoad(void);
		virtual UINT getLoadedResourceSize(char *p_RawBuffer, UINT p_RawSize);
		virtual bool loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle);
		virtual std::string getPattern(void);
	};
}