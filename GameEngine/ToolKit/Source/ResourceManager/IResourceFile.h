#pragma once

#include <string>

class Resource;

class IResourceFile
{
public:
	virtual bool open() = 0;
	virtual int getRawResourceSize(const Resource &p_R) = 0;
	virtual int getRawResource(const Resource &p_R, char *p_Buffer) = 0;
	virtual int getNumResources() const = 0;
	virtual std::string getResourceName(int p_Num) const = 0;
	virtual bool isUsingDevelopmentDirectories(void) const = 0;
	virtual ~IResourceFile() { }
};