#pragma once

#include "LinearAllocator.h"

namespace Allocator
{
	class LinearDeleter
	{
		public:
			template<typename T>
			void operator()(T *p)
			{
				p->~Destruct();
			}
	};
}