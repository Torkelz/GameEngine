#include "ResHandle.h"
#include "ResourceManager.h"
#include "Macros.h"

//
// ResHandle::ResHandle							- Chapter 8, page 223
//
ResHandle::ResHandle(Resource & p_Resource, char *p_Buffer, unsigned int p_Size, ResourceManager *p_ResCache)
: m_Resource(p_Resource)
{
	m_Buffer = p_Buffer;
	m_Size = p_Size;
	m_Extra = NULL;
	m_ResCache = p_ResCache;
}

//
// ResHandle::ResHandle							- Chapter 8, page 223
//
ResHandle::~ResHandle()
{
	SAFE_DELETE_ARRAY(m_Buffer);
	m_ResCache->memoryHasBeenFreed(m_Size);
}

const std::string ResHandle::getName() 
{
	return m_Resource.m_Name;
}

unsigned int ResHandle::size() const 
{ 
	return m_Size; 
}

char *ResHandle::buffer() const
{ 
	return m_Buffer; 
}

char *ResHandle::writableBuffer() 
{ 
	return m_Buffer;
}

std::shared_ptr<IResourceExtraData> ResHandle::getExtra() 
{ 
	return m_Extra;
}

void ResHandle::setExtra(std::shared_ptr<IResourceExtraData> extra)
{
	m_Extra = extra;
}