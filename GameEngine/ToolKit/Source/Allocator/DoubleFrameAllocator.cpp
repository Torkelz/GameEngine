#include "DoubleFrameAllocator.h"

Allocator::DoubleFrameAllocator::DoubleFrameAllocator(UINT p_Size)
{
	m_Allocators[0] = new LinearAllocator(p_Size);
	m_Allocators[1] = new LinearAllocator(p_Size);
	m_Current = 0;
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
	m_Allocators[m_Current]->clear();
}

void *Allocator::DoubleFrameAllocator::allocate(UINT p_Size)
{
	return m_Allocators[m_Current]->allocate(p_Size, 4);
}

