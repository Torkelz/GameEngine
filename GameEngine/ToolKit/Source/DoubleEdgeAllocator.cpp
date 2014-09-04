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

	void DoubleEdgeAllocator::freeToMarker(const Marker &p_Marker)
	{}

	void DoubleEdgeAllocator::clear()
	{}

	std::pair<DoubleEdgeAllocator::Edge, DoubleEdgeAllocator::UINT> DoubleEdgeAllocator::getMarker(const DoubleEdgeAllocator::Edge &p_Edge) const
	{
		return std::make_pair<Edge, UINT>(p_Edge, m_Marker[p_Edge]);
	}
}
