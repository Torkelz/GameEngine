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

	PoolAllocator::PoolAllocator(char *p_Buffer, UINT p_ItemSize, UINT p_NumItem)
	{
		//m_Data = p_Buffer;
		
		//TODO: Move pointer in buffer forwards and such
		//m_Marker = m_Data;
		//m_ItemSize = p_ItemSize;
	}

	PoolAllocator::~PoolAllocator()
	{
		if(m_Data)
			free(m_Data);

		m_Marker = nullptr;
	}

	void *PoolAllocator::allocate(void)
	{
		// If marker is nullptr, no more segments are available.
		if (m_Marker == nullptr)
			return nullptr;

		// Allocate next free segment and change the marker to next free segment.
		void *freeMemory = m_Marker;
		m_Marker = (void**)(*m_Marker);
		return freeMemory;
	}

	void PoolAllocator::deallocate(void *p_Position)
	{
		*((void**)p_Position) = m_Marker;
		m_Marker = (void**)p_Position;
	}

	void PoolAllocator::clear()
	{
		//Setting each segment to mark where next segment starts.
		void** p = (void**)m_Data;

		//Initialize free blocks list
		for (size_t i = 0; i < m_NumItems - 1; i++)
		{
			*p = (void*)(reinterpret_cast<uintptr_t>(p)+m_ItemSize);
			p = (void**)*p;
		}
		//The last pointer in the list is set to nullptr
		*p = nullptr;
	}
}
