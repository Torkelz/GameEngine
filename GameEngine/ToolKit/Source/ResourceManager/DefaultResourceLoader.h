#pragma once
#include "IResourceLoader.h"

//
// class DefaultResourceLoader							- Chapter 8, page 225
//
class DefaultResourceLoader : public IResourceLoader
{
public:
	virtual bool useRawFile();
	virtual bool discardRawBufferAfterLoad();
	virtual unsigned int getLoadedResourceSize(char *p_RawBuffer, unsigned int p_RawSize);
	virtual bool loadResource(char *p_RawBuffer, unsigned int p_RawSize, std::shared_ptr<ResHandle> p_Handle);
	virtual std::string getPattern();
};