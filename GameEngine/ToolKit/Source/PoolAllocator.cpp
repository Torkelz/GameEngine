#include "MemoryExceptions.h"
#include "PoolAllocator.h"
#include <malloc.h>
#include <string>


namespace Allocator
{
	PoolAllocator::PoolAllocator(UINT p_ItemSize, UINT p_NumItem)
	{
		UINT totalSize = p_ItemSize * p_NumItem;
		m_Data = (char*)malloc(totalSize);
		if (!m_Data)
			throw MemoryException("Bad allocation at: ", __LINE__, __FILE__);

		//m_nrAllocations = 0;
		//m_Size = p_NumItem;
		m_Marker = (void**)m_Data;

		// Setting each segment to mark where next segment starts.
		void** tempMarker = m_Marker;
		for (UINT i = 0; i < p_NumItem - 1; i += p_ItemSize)
		{
			*tempMarker = tempMarker + p_ItemSize;
			tempMarker = (void**) *tempMarker;
		}

		// The last pointer in the list is set to nullptr
		*tempMarker = tempMarker + p_ItemSize;
		tempMarker = nullptr;
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
		m_Marker = nullptr;
	}
}
