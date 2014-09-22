#pragma once
#include "IResourceLoader.h"

//
// class DefaultResourceLoader							- Chapter 8, page 225
//
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
		virtual bool loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResHandle> p_Handle);
		virtual std::string getPattern(void);
	};
}