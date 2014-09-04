#include "LinearAllocator.h"
#include "MemoryExceptions.h"

#include <malloc.h>
#include <string>

namespace Allocator
{
	LinearAllocator::LinearAllocator(UINT p_Size)
		: m_Size(p_Size)
	{
		if (m_Size == 0)
			throw MemoryException("LinearAllocator, size was 0", __LINE__, __FILE__);

		m_Buffer = (char*)malloc(m_Size);
		if (m_Buffer == nullptr)
			throw MemoryException("LinearAllocator, failed to allocate memory", __LINE__, __FILE__);
	}

	LinearAllocator::LinearAllocator(char* p_Buffer, UINT p_Size)
		: m_Buffer(p_Buffer), m_Size(p_Size)
	{
		if (m_Size == 0)
			throw MemoryException("LinearAllocator, size was 0", __LINE__, __FILE__);
		if (m_Buffer == nullptr)
			throw MemoryException("LinearAllocator, buffer parameter was nullptr", __LINE__, __FILE__);
	}

	LinearAllocator::~LinearAllocator()
	{
	}

	void* LinearAllocator::alloc(UINT p_Size)
	{

	}

	void LinearAllocator::freeToMarker(const UINT &p_Marker)
	{

	}

	void LinearAllocator::clear()
	{

	}

	const LinearAllocator::UINT& LinearAllocator::getMarker() const
	{

	}
}