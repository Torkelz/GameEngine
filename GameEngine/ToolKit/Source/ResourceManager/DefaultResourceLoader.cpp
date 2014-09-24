#include "DefaultResourceLoader.h"

namespace Res
{
	bool DefaultResourceLoader::useRawFile(void)
	{
		return true;
	}

	bool DefaultResourceLoader::discardRawBufferAfterLoad(void)
	{
		return true;
	}

	DefaultResourceLoader::UINT DefaultResourceLoader::getLoadedResourceSize(char *p_RawBuffer, UINT p_RawSize)
	{
		return p_RawSize;
	}

	bool DefaultResourceLoader::loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle)
	{
		return true;
	}

	std::string DefaultResourceLoader::getPattern(void)
	{
		return "*";
	}
}