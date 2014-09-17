#include "Assignment1.h"


#ifdef OLDNEW
#define NEW(x) new
#else
#define NEW(x) new(x)
#endif

#ifdef OLDNEW
#define DELETE(x, y) if(x != nullptr){ delete x; x = nullptr;}
#else
#define DELETE(x, y) if(x != nullptr){operator delete(x, y); x = nullptr;}
#endif


Assignment1::Assignment1(void)
{
}


Assignment1::~Assignment1(void)
{
}
int *k[2049];

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

	//Cleanup for old new
#ifdef OLDNEW
	for (unsigned int i = 0; i < p_NumItems; i++)
	{
		DELETE(k[i], nullptr);
	}
#endif
}

void Assignment1::scenario2(unsigned int p_Size)
{
	Allocator::LinearAllocator oneFrameStack(p_Size);
	int j = 0;
	int *b[512];

	while (j < 5000)
	{
		oneFrameStack.clear();

		for (int i = 0; i < 512; i++)
		{
			b[i] = NEW(oneFrameStack) int;
			
			if (b[i] != nullptr)
				*b[i] = i + j;
		}

		for (int i = 1; i < 512; i++)
		{
			if (b[i] != nullptr)
				*b[i] = *b[i] * *b[i - 1];
			else
				break;
		}

		j++;
	}
}

