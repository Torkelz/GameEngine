#pragma once

namespace Allocator
{
	class PoolAllocator
	{
		typedef unsigned int UINT;

	private:
		char *m_Data;
		void *m_FirstFree;
		UINT m_ItemSize;

	public:
		PoolAllocator(UINT p_ItemSize, UINT p_NumItem);
		PoolAllocator(char *p_Buffer, UINT p_ItemSize, UINT p_NumItem);
		~PoolAllocator();
		void *allocate(void);
		void deallocate(void *p_Position);
		void clear(void);
	};
}

