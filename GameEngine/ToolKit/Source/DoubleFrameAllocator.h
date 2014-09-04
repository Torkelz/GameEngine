#pragma once
#include "LinearAllocator.h"

namespace Allocator
{
	typedef unsigned int UINT;

	class DoubleFrameAllocator
	{
	private:
		LinearAllocator m_Allocators[2];
		UINT m_Current;

	public:
		DoubleFrameAllocator(UINT p_Size);
		~DoubleFrameAllocator();

		void swap();
		void clearCurrentBuffer();
		void *alloc(UINT p_Size);
	};
}