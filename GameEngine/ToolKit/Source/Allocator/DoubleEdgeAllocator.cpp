#include "DoubleEdgeAllocator.h"
#include "MemoryExceptions.h"

namespace Allocator
{
	DoubleEdgeAllocator::DoubleEdgeAllocator(UINT p_Size)
		: m_Size(p_Size), m_Original(true), m_BottomMarker(0), m_TopMarker(p_Size - 1)
	{
		if (m_Size == 0)
			throw MemoryException("LinearAllocator, size was 0", __LINE__, __FILE__);

		m_Buffer = (char*)malloc(m_Size + sizeof(void*));
		if (m_Buffer == nullptr)
			throw MemoryException("LinearAllocator, failed to allocate memory", __LINE__, __FILE__);
	}

	DoubleEdgeAllocator::DoubleEdgeAllocator(char *p_Buffer, UINT p_Size)
		: m_Buffer(p_Buffer), m_Size(p_Size), m_Original(false), m_BottomMarker(0), m_TopMarker(p_Size - 1)
	{
		if (m_Size == 0)
			throw MemoryException("LinearAllocator, size was 0", __LINE__, __FILE__);
		if (m_Buffer == nullptr)
			throw MemoryException("LinearAllocator, buffer parameter was nullptr", __LINE__, __FILE__);
	}

	DoubleEdgeAllocator::~DoubleEdgeAllocator(void)
	{
		/**
		* Memory should be returned to the OS if this is where the memory was originally allocated.
		* Else set to nullptr not to mess up for others.
		*/
		if (m_Original)
			free(m_Buffer);

		m_Buffer = nullptr;
	}

	void *DoubleEdgeAllocator::allocate(UINT p_Size, DoubleEdgeAllocator::Edge p_Edge)
	{
		switch (p_Edge)
		{
		case Edge::TOP:
		{
			if (m_TopMarker.load() - p_Size <= m_BottomMarker)
				return nullptr;

			void *currentAdress = m_Buffer + m_TopMarker.fetch_sub(p_Size) - p_Size;

			return currentAdress;
		}
		case Edge::BOTTOM:
		{
			if (m_BottomMarker.load() + p_Size >= m_TopMarker)
				return nullptr;

			void *currentAdress = m_Buffer + m_BottomMarker.fetch_add(p_Size) + p_Size;

			return currentAdress;
		}
		default:
			return nullptr;
		}
	}

	void DoubleEdgeAllocator::freeTopMarkerTo(UINT p_Marker)
	{
		m_TopMarker = p_Marker;
	}

	void DoubleEdgeAllocator::freeBottomMarkerTo(UINT p_Marker)
	{
		m_BottomMarker = p_Marker;
	}

	void DoubleEdgeAllocator::clear(void)
	{
		// Bottom is lowest position, top is highest.
		m_BottomMarker = 0;
		m_TopMarker = m_Size - 1;
	}

	DoubleEdgeAllocator::UINT DoubleEdgeAllocator::getTopMarker(void) const
	{
		return m_TopMarker.load();
	}
	DoubleEdgeAllocator::UINT DoubleEdgeAllocator::getBottomMarker(void) const
	{
		return m_BottomMarker.load();
	}
}
