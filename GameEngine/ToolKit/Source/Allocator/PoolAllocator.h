#pragma once
#include <atomic>

namespace Allocator
{
	class PoolAllocator
	{
		typedef unsigned int UINT;

	private:
		/**
		 * Memory.
		 */
		char *m_Data;

		/**
		 * Marks the next free block in memory.
		 */
		std::atomic<void**> m_Marker;

		/**
		* Block size.
		*/
		UINT m_ItemSize;

		/**
		 * Number of slots.
		 */
		UINT m_NumItems;

	public:
		/**
		 * Constructor if the pool allocator is not going to use memory already allocated.
		 * @param ItemSize, the size of one item
		 * @param NumItems, the desired number of items to fit in memory
		 */
		PoolAllocator(UINT p_ItemSize, UINT p_NumItems);

		/**
		 * Constructor if the pool allocator is going to use memory already allocated.
		 * @param Buffer, pointer to the memory where the allocators memory pool will be placed
		 * @param ItemSize, the size of one item
		 * @param NumItems, the desired number of items to fit in memory
		 *
		 * WARNING: Not implemented yet!
		 */
		PoolAllocator(char *p_Buffer, UINT p_ItemSize, UINT p_NumItems);

		/**
		 * Destructor.
		 */
		~PoolAllocator();

		/**
		 * Allocate a block in the memory.
		 * @return, the memory adress that has been allocated. nullptr is returned if memory is full.
		 */
		void *allocate(void);

		/**
		 * Return a memory block to the free list.
		 * @param Position, the memory adress to be returned to the free list
		 */
		void deallocate(void *p_Position);

		/**
		 * Clear the whole pool.
		 */
		void clear(void);

	private:
		PoolAllocator(const PoolAllocator& p_Other) = delete; // non construction-copyable
		PoolAllocator& operator=(const PoolAllocator&) = delete; // non copyable
	};
}

inline void* operator new (size_t p_Size, Allocator::PoolAllocator &p_Allocator)
{
	return p_Allocator.allocate();
}

inline void operator delete (void* p_Position, Allocator::PoolAllocator &p_Allocator)
{
	p_Allocator.deallocate(p_Position);
}
