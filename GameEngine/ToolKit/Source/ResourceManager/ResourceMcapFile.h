#pragma once
#include "McapFile.h"
#include "IResourceFile.h"

namespace Res
{
	class ResourceMcapFile : public IResourceFile
	{
		McapFile *m_McapFile;
		std::wstring m_ResFileName;

	public:
		ResourceMcapFile();
		virtual ~ResourceMcapFile(void);

		virtual void initialize(std::wstring p_ResFileName);
		virtual bool open(void);
		virtual int getRawResourceSize(const Resource &p_R);
		virtual int getRawResource(const Resource &p_R, char *p_Buffer);
		virtual int getNumResources(void) const;
		virtual std::string getResourceName(int p_Num) const;
		virtual std::string getFilePath(void) const;
	};
}