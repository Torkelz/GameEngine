#pragma once

#include <string>
#include <memory>

#include "Resource.h"

class IResourceExtraData;

//
//  class ResHandle			- Chapter 8, page 222
//
class ResHandle
{
	friend class ResourceManager;

protected:
	Resource m_Resource;
	char *m_Buffer;
	unsigned int m_Size;
	std::shared_ptr<IResourceExtraData> m_Extra;
	ResourceManager *m_ResCache;

public:
	ResHandle(Resource &p_Resource, char *p_Buffer, unsigned int p_Size, ResourceManager *p_ResCache);

	virtual ~ResHandle();

	const std::string getName();
	unsigned int size() const;
	char *buffer() const;
	char *writableBuffer();

	std::shared_ptr<IResourceExtraData> getExtra();
	void setExtra(std::shared_ptr<IResourceExtraData> extra);
};