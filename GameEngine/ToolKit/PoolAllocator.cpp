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
		{
			std::string errorMsg = "Bad allocation at line: " + std::to_string(__LINE__) + ", in file: " + __FILE__;
			throw errorMsg;
		}
		m_FirstFree = m_Data;
		m_ItemSize = p_ItemSize;

		for (UINT i = 0; i < totalSize; i += p_ItemSize)
		{
			m_Data[i] = +p_ItemSize;
		}
	}

	PoolAllocator::PoolAllocator(char *p_Buffer, UINT p_ItemSize, UINT p_NumItem)
	{
		m_Data = p_Buffer;
		
		//TODO: Move pointer in buffer forwards and such
		m_FirstFree = m_Data;
		m_ItemSize = p_ItemSize;
	}

	PoolAllocator::~PoolAllocator()
	{
	}

	void *PoolAllocator::allocate(void)
	{
		//This is stack based allocation! do not use here!
		//void *allocation = m_Data;
		//m_Data += m_ItemSize;

		//return allocation;

		//TODO: Pick out next item in free list and return it?

		//return next item in free list
		return nullptr;
	}

	void PoolAllocator::deallocate(void *p_Position)
	{

	}

	void PoolAllocator::clear()
	{

	}
}
