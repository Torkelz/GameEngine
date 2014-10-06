#pragma once
#include "IResourceFile.h"

#include <string>

namespace Res
{
	class ZipFile;

	//
	//  class ResourceZipFile	- not discussed in the book
	//
	//    This class implements the IResourceFile interface with a ZipFile.
	class ResourceZipFile : public IResourceFile
	{
		ZipFile *m_ZipFile;
		std::wstring m_ResFileName;

	public:
		ResourceZipFile(const std::wstring p_ResFileName);
		virtual ~ResourceZipFile(void);

		virtual bool open(void);
		virtual int getRawResourceSize(const Resource &p_R);
		virtual int getRawResource(const Resource &p_R, char *p_Buffer);
		virtual int getNumResources(void) const;
		virtual std::string getResourceName(int p_Num) const;
		//virtual bool isUsingDevelopmentDirectories(void) const { return false; }
		virtual std::string getFilePath(void) const;
	};
}