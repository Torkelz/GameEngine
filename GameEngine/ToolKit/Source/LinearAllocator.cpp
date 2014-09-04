#include "LinearAllocator.h"

namespace Allocator
{
	LinearAllocator::LinearAllocator(UINT p_Size)
	{
	}


	LinearAllocator::~LinearAllocator()
	{
	}

	void* LinearAllocator::alloc(UINT p_Size)
	{
		return nullptr;
	}

	void LinearAllocator::freeToMarker(const UINT &p_Marker)
	{

	}

	void LinearAllocator::clear()
	{

	}

	const LinearAllocator::UINT& LinearAllocator::getMarker() const
	{
		return m_Marker;
	}
}