#pragma once

#include <string>

namespace Res
{
	class Resource;

	class IResourceFile
	{
	public:
		virtual void initialize(std::wstring p_ResFileName) = 0;
		virtual bool open(void) = 0;
		virtual int getRawResourceSize(const Resource &p_R) = 0;
		virtual int getRawResource(const Resource &p_R, char *p_Buffer) = 0;
		virtual int getNumResources(void) const = 0;
		virtual std::string getResourceName(int p_Num) const = 0;
		virtual ~IResourceFile(void) { }
		virtual std::string getFilePath(void) const = 0;
	};
}