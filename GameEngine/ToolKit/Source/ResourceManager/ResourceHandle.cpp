#include "ResourceHandle.h"
#include "ResourceManager.h"
#include "Macros.h"

namespace Res
{
	//
	// ResourceHandle::ResourceHandle							- Chapter 8, page 223
	//
	ResourceHandle::ResourceHandle(Resource &p_Resource, char *p_Buffer, UINT p_Size, ResourceManager *p_ResCache)
		: m_Resource(p_Resource), m_Buffer(p_Buffer), m_Size(p_Size), m_Extra(nullptr), m_ResCache(p_ResCache)
	{
	}

	//
	// ResourceHandle::ResourceHandle							- Chapter 8, page 223
	//
	ResourceHandle::~ResourceHandle(void)
	{
		SAFE_DELETE_ARRAY(m_Buffer);
		m_ResCache->memoryHasBeenFreed(m_Size);
	}

	const std::string ResourceHandle::getName(void)
	{
		return m_Resource.m_Name;
	}

	ResourceHandle::UINT ResourceHandle::size(void) const
	{
		return m_Size;
	}

	char *ResourceHandle::buffer(void) const
	{
		return m_Buffer;
	}

	char *ResourceHandle::writableBuffer(void)
	{
		return m_Buffer;
	}

	std::shared_ptr<IResourceExtraData> ResourceHandle::getExtra(void)
	{
		return m_Extra;
	}

	void ResourceHandle::setExtra(std::shared_ptr<IResourceExtraData> extra)
	{
		m_Extra = extra;
	}
}