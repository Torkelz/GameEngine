#include "PoolAllocator.h"


Allocator::PoolAllocator::PoolAllocator(UINT p_ItemSize, UINT p_NumItem)
{
}


Allocator::PoolAllocator::~PoolAllocator()
{
}

void *Allocator::PoolAllocator::allocate(void)
{
	return nullptr;
}

void Allocator::PoolAllocator::deallocate(void *p_Position)
{
}

void Allocator::PoolAllocator::clear()
{
}