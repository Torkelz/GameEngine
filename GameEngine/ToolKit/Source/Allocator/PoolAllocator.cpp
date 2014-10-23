#include "MemoryExceptions.h"
#include "PoolAllocator.h"
#include <malloc.h>
#include <string>

namespace Allocator
{
	PoolAllocator::PoolAllocator(UINT p_ItemSize, UINT p_NumItems)
		: m_Original(true)
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

	PoolAllocator::PoolAllocator(UINT p_ItemSize, UINT p_ObjectAlignment, UINT p_NumItems)
		: m_Original(true)
	{
		UINT totalSize = p_ItemSize * p_NumItems;
		m_Data = (char*)malloc(totalSize);
		if (!m_Data)
			throw MemoryException("Bad allocation at: ", __LINE__, __FILE__);

		UINT adjustment = alignForwardAdjustment((void*)m_Data, p_ObjectAlignment);

		m_ItemSize = p_ItemSize;
		m_NumItems = (totalSize - adjustment) / m_ItemSize;
		m_Marker = (void**)((uintptr_t)m_Data + adjustment);

		//Setting each segment to mark where next segment starts.
		void** p = (void**)((uintptr_t)m_Data + adjustment);

		//Initialize free blocks list
		for (size_t i = 0; i < m_NumItems - 1; i++)
		{
			*p = (void*)(reinterpret_cast<uintptr_t>(p)+m_ItemSize);
			p = (void**)*p;
		}
		//The last pointer in the list is set to nullptr
		*p = nullptr;
	}

	PoolAllocator::PoolAllocator(char *p_Buffer, UINT p_ItemSize, UINT p_NumItems)
		: m_Original(false)
	{
		m_Data = p_Buffer;
		
		m_ItemSize = p_ItemSize;
		m_NumItems = p_NumItems;
		m_Marker = (void**)m_Data;

		clear();
	}


	PoolAllocator::~PoolAllocator()
	{
		if(m_Data && m_Original)
			free(m_Data);
		m_Data = nullptr;
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


	PoolAllocator::UINT PoolAllocator::alignForwardAdjustment(const void *p_Address, UINT p_Alignment)
	{
		UINT adjustment = p_Alignment - (reinterpret_cast<uintptr_t>(p_Address)& static_cast<uintptr_t>(p_Alignment - 1));

		if (adjustment == p_Alignment)
			return 0; // already aligned

		return adjustment;
	}
}
