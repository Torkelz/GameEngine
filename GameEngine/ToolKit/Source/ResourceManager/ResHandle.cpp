#include "ResHandle.h"
#include "ResourceManager.h"
#include "Macros.h"

namespace Res
{
	//
	// ResHandle::ResHandle							- Chapter 8, page 223
	//
	ResHandle::ResHandle(Resource &p_Resource, char *p_Buffer, UINT p_Size, ResourceManager *p_ResCache)
		: m_Resource(p_Resource), m_Buffer(p_Buffer), m_Size(p_Size), m_Extra(nullptr), m_ResCache(p_ResCache)
	{
	}

	//
	// ResHandle::ResHandle							- Chapter 8, page 223
	//
	ResHandle::~ResHandle(void)
	{
		SAFE_DELETE_ARRAY(m_Buffer);
		m_ResCache->memoryHasBeenFreed(m_Size);
	}

	const std::string ResHandle::getName(void)
	{
		return m_Resource.m_Name;
	}

	ResHandle::UINT ResHandle::size(void) const
	{
		return m_Size;
	}

	char *ResHandle::buffer(void) const
	{
		return m_Buffer;
	}

	char *ResHandle::writableBuffer(void)
	{
		return m_Buffer;
	}

	std::shared_ptr<IResourceExtraData> ResHandle::getExtra(void)
	{
		return m_Extra;
	}

	void ResHandle::setExtra(std::shared_ptr<IResourceExtraData> extra)
	{
		m_Extra = extra;
	}
}