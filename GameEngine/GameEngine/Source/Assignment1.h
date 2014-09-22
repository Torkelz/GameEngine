#pragma once

#include "IAllocators.h"

class Assignment1
{
public:
	Assignment1(void);
	~Assignment1(void);

	void scenario1(unsigned int p_ItemSize, unsigned int p_NumItems);
	void scenario2(unsigned int p_Size);

	void threadTestPoolAllocator();

	void thread1test(Allocator::PoolAllocator&);
	void thread2test(Allocator::PoolAllocator&);
};

