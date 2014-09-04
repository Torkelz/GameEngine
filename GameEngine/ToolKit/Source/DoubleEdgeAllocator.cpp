#include "DoubleEdgeAllocator.h"

namespace Allocator
{
	DoubleEdgeAllocator::DoubleEdgeAllocator(UINT p_Size)
	{}

	DoubleEdgeAllocator::DoubleEdgeAllocator(char *p_Buffer, UINT p_Size)
	{}

	DoubleEdgeAllocator::~DoubleEdgeAllocator()
	{}

	void *DoubleEdgeAllocator::alloc(UINT p_Size, DoubleEdgeAllocator::Edge p_Edge)
	{}

	void DoubleEdgeAllocator::freeToTopMarker(const UINT &p_Marker)
	{
	
	}

	void DoubleEdgeAllocator::freeToBottomMarker(const UINT &p_Marker)
	{

	}

	void DoubleEdgeAllocator::clear()
	{}

	DoubleEdgeAllocator::UINT DoubleEdgeAllocator::getTopMarker() const
	{
		return m_TopMarker;
	}
	DoubleEdgeAllocator::UINT DoubleEdgeAllocator::getBottomMarker() const
	{
		return m_BottomMarker;
	}
}
