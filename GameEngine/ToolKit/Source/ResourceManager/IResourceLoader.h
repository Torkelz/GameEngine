#pragma once

#include <string>
#include <memory>

namespace Res
{
	class ResourceHandle;

	class IResourceLoader
	{
	public:
		typedef unsigned int UINT;
	public:
		virtual std::string getPattern(void) = 0;
		virtual bool useRawFile(void) = 0;
		virtual bool discardRawBufferAfterLoad(void) = 0;
		virtual bool addNullZero(void) { return false; }
		virtual UINT getLoadedResourceSize(char *p_RawBuffer, UINT p_RawSize) = 0;
		virtual bool loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle) = 0;
	};
}