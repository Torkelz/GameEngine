#pragma once
#include "LinearAllocator.h"

namespace Allocator
{
	typedef unsigned int UINT;

	class DoubleFrameAllocator
	{
	private:
		/**
		 * The two stack allocators used to do the two frame buffering.
		 */
		LinearAllocator *m_Allocators[2];

		/**
		 * Currently active buffer.
		 */
		UINT m_Current;

	public:
		/**
		 * Contruct the double frame buffer.
		 * @param Size, the size of one of the two buffers. Both buffer will have the same size.
		 */
		DoubleFrameAllocator(UINT p_Size);

		/**
		 * Deconstruct the double frame buffer.
		 */
		~DoubleFrameAllocator();

		/**
		 * Change active buffer.
		 */
		void swap();

		/**
		 * Clear the active buffer. In the beginning of a frame is a good time to do this.
		 */
		void clearCurrentBuffer();

		/**
		 * Allocate a block in the the current buffer and save it to the next frame.
		 * @param Size, the memory size to be allocated.
		 * @return, the memory adress that has been allocated. nullptr is returned if memory is full.
		 */
		void *allocate(UINT p_Size);

	private:
		DoubleFrameAllocator(const DoubleFrameAllocator& p_Other) = delete; // non construction-copyable
		DoubleFrameAllocator& operator=(const DoubleFrameAllocator&) = delete; // non copyable
	};
}

inline void* operator new (size_t p_Size, Allocator::DoubleFrameAllocator &p_Allocator)
{
	return p_Allocator.allocate(p_Size);
}