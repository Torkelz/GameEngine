#include "MemoryExceptions.h"
#include "PoolAllocator.h"
#include <malloc.h>
#include <string>

namespace Allocator
{
	PoolAllocator::PoolAllocator(UINT p_ItemSize, UINT p_NumItems)
	{
		UINT totalSize = p_ItemSize * p_NumItems;
		m_Data = (char*)malloc(totalSize);
		if (!m_Data)
			throw MemoryException("Bad allocation at: ", __LINE__, __FILE__);

		m_ItemSize = p_ItemSize;
		m_NumItems = p_NumItems;
		m_Marker = (void**)m_Data;

		clear();
	}

	PoolAllocator::PoolAllocator(char *p_Buffer, UINT p_ItemSize, UINT p_NumItems)
	{
		m_Data = p_Buffer;
		
		m_ItemSize = p_ItemSize;
		m_NumItems = p_NumItems;
		m_Marker = (void**)m_Data;

		clear();
	}

	PoolAllocator::~PoolAllocator()
	{
		if(m_Data)
			free(m_Data);

		m_Marker = nullptr;
	}

	void *PoolAllocator::allocate(void)
	{
		void *freeMemory = m_Marker.load(std::memory_order_acquire);

		// If marker is nullptr, no more segments are available.
		if (!freeMemory)
			return nullptr;

		// Allocate next free segment and change the marker to next free segment.
		m_Marker.store((void**)(*m_Marker), std::memory_order_release);
		
		return freeMemory;
	}

	void PoolAllocator::deallocate(void *p_Position)
	{
		if (!p_Position)
			return;

		*((void**)p_Position) = m_Marker.load(std::memory_order_acquire);
		m_Marker.store((void**)p_Position, std::memory_order_release);
	}

	void PoolAllocator::clear()
	{
		//Setting each segment to mark where next segment starts.
		void** p = (void**)m_Data;

		//Initialize free blocks list
		for (size_t i = 0; i < m_NumItems - 1; i++)
		{
			*p = (void*)(reinterpret_cast<uintptr_t>(p) + m_ItemSize);
			p = (void**)*p;
		}
		//The last pointer in the list is set to nullptr
		*p = nullptr;
	}
}
