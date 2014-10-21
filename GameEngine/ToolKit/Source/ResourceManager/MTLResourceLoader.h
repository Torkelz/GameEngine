#pragma once
#include "IResourceLoader.h"
#include "MTLResourceExtraData.h"
#include <DirectXMath.h>

namespace Res
{
	class MTLResourceLoader : public IResourceLoader
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
		struct membuf : std::streambuf
		{
			membuf(char* begin, char* end) {
				this->setg(begin, begin, end);
			}
		};

		bool parseMTL(char *p_MtlStream, size_t p_Length, std::shared_ptr<ResourceHandle> p_Handle);
	};
}

