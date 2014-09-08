#pragma once

namespace Allocator
{
	class PoolAllocator
	{
		typedef unsigned int UINT;

	private:
		char *m_Data;
		void **m_Marker;
		//UINT m_Size;
		//UINT m_nrAllocations;

	public:
		PoolAllocator(UINT p_ItemSize, UINT p_NumItem);
		PoolAllocator(char *p_Buffer, UINT p_ItemSize, UINT p_NumItem);
		~PoolAllocator();
		void *allocate(void);
		void deallocate(void *p_Position);
		void clear(void);
	};
}

