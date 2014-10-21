#include "ResourceHandle.h"
#include "ResourceManager.h"
#include "Macros.h"

namespace Res
{
	ResourceHandle::ResourceHandle(Resource &p_Resource, char *p_Buffer, UINT p_Size, ResourceManager *p_ResCache)
		: m_Resource(p_Resource), m_Buffer(p_Buffer), m_Size(p_Size), m_Extra(nullptr), m_ResCache(p_ResCache)
	{
	}

	ResourceHandle::~ResourceHandle(void)
	{
		m_ResCache->memoryHasBeenFreed(m_Size, m_Resource.m_Name);
		if (m_Buffer)
			delete m_Buffer;
	}

	const std::string &ResourceHandle::getName(void) const
	{
		return m_Resource.m_Name;
	}

	const std::string &ResourceHandle::getContainerName(void) const
	{
		return m_Resource.m_ZipName;
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

	std::shared_ptr<IResourceExtraData> ResourceHandle::getExtra(void) const
	{
		return m_Extra;
	}

	void ResourceHandle::setExtra(std::shared_ptr<IResourceExtraData> extra)
	{
		m_Extra = extra;
	}
}