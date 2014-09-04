#include "DoubleFrameAllocator.h"

Allocator::DoubleFrameAllocator::DoubleFrameAllocator()
{
}

Allocator::DoubleFrameAllocator::~DoubleFrameAllocator()
{
}

void Allocator::DoubleFrameAllocator::swap()
{
	if (m_Current == 0)
	{
		m_Current++;
	}	
	else
	{
		m_Current--;
	}
}

void Allocator::DoubleFrameAllocator::clearCurrentBuffer()
{
	m_Allocators[m_Current].clear();
}

void *Allocator::DoubleFrameAllocator::alloc(UINT p_Size)
{
	return m_Allocators[m_Current].alloc(p_Size);
}