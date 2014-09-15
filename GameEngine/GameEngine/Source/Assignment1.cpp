#include "Assignment1.h"

#define OLDNEW

#ifdef OLDNEW
#define NEW(x) new
#else
#define NEW(x) new(x)
#endif


Assignment1::Assignment1(void)
{
}


Assignment1::~Assignment1(void)
{
}

void Assignment1::scenario2(unsigned int p_Size)
{
	Allocator::LinearAllocator oneFrameStack(p_Size);
	int j = 0;
	while (j < 5000)
	{
		oneFrameStack.clear();

		int *k[512];
		for (int i = 0; i < 512; i++)
		{
			k[i] = NEW(oneFrameStack) int;
			
			if (k[i] != nullptr)
				*k[i] = i + j + 235;
		}

		for (int i = 1; i < 512; i++)
		{
			if (k[i] != nullptr)
				*k[i] = *k[i] * *k[i - 1];
			else
				break;
		}

		j++;
	}
}

