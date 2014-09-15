#include "Assignment1.h"

#define OLDNEW

#ifdef OLDNEW
#define NEW(x) new
#else
#define NEW(x) new(x)
#endif

#ifdef OLDNEW
#define DELETE(x, y) delete x
#else
#define DELETE(x, y) operator delete(x, y)
#endif

int *k[2048];

Assignment1::Assignment1(void)
{
}


Assignment1::~Assignment1(void)
{
}

void Assignment1::scenario1(unsigned int p_ItemSize, unsigned int p_NumItems)
{
	Allocator::PoolAllocator memoryPool(p_ItemSize, p_NumItems);
	int j = 0;
	for (unsigned int i = 0; i < p_NumItems; i++)
	{
		k[i] = NEW(memoryPool) int;
		if (k[i] != nullptr)
			*k[i] = i + j;
	}
	unsigned int removed = 0;
	while (j < 5000)
	{
		if (j % 100 == 0)
		{
			DELETE(k[removed], memoryPool);
			removed += 5;

			if (removed >= p_NumItems)
			{
				removed = removed % p_NumItems;
			}
		}
		j++;
	}
	//for (unsigned int i = 0; i < p_NumItems; i++)
	//{
	//	delete k[i];
	//}
	free(k);
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
				*k[i] = i + j;
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

