#include "DefaultResourceLoader.h"

bool DefaultResourceLoader::useRawFile() 
{ 
	return true;
}

bool DefaultResourceLoader::discardRawBufferAfterLoad() 
{ 
	return true; 
}

unsigned int DefaultResourceLoader::getLoadedResourceSize(char *p_RawBuffer, unsigned int p_RawSize) 
{
	return p_RawSize; 
}

bool DefaultResourceLoader::loadResource(char *p_RawBuffer, unsigned int p_RawSize, std::shared_ptr<ResHandle> p_Handle)
{ 
	return true; 
}

std::string DefaultResourceLoader::getPattern() 
{ 
	return "*";
}