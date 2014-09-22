#pragma once

#include <string>
#include <memory>

class ResHandle;

//
// class IResourceLoader					- Chapter 8, page 224
//
class IResourceLoader
{
public:
	virtual std::string getPattern() = 0;
	virtual bool useRawFile() = 0;
	virtual bool discardRawBufferAfterLoad() = 0;
	virtual bool addNullZero() { return false; }
	virtual unsigned int getLoadedResourceSize(char *p_RawBuffer, unsigned int p_RawSize) = 0;
	virtual bool loadResource(char *p_RawBuffer, unsigned int p_RawSize, std::shared_ptr<ResHandle> p_Handle) = 0;
};