#pragma once

#include "IToolKit.h"

#include <vector>

//#define OLDNEW

#ifdef OLDNEW
#define DBGNEW(x) new
#else
#define DBGNEW(x) new(x)
#endif

#ifdef OLDNEW
#define DBGDELETE(x, y) if(x != nullptr){ delete x; x = nullptr;}
#else
#define DBGDELETE(x, y) if(x != nullptr){operator delete(x, y); x = nullptr;}
#endif



class Assignment1
{
public:
	Assignment1(void){};
	~Assignment1(void){};

	template<typename T>
	void scenario1(unsigned int p_NumItems )
	{
		std::vector<T*> buffer1(p_NumItems, nullptr);
		Allocator::PoolAllocator memoryPool(sizeof(T), p_NumItems);

		for (unsigned int i = 0; i < p_NumItems; i++)
		{
			buffer1[i] = DBGNEW(memoryPool) T;
			if (buffer1[i] != nullptr)
				*buffer1[i] = i;
		}

		unsigned int removed = 0;
		int j = 0;
		while (j < 5000)
		{
			if (j % 100 == 0)
			{
				DBGDELETE(buffer1[removed], memoryPool);
				removed += 5;

				if (removed >= p_NumItems)
				{
					removed = removed % p_NumItems;
				}
			}
			j++;
		}

	//#ifdef OLDNEW
		for (unsigned int i = 0; i < p_NumItems; i++)
		{
			DBGDELETE(buffer1[i], memoryPool);
		}
	//#endif
	}

	template<typename T>
	void scenario2(unsigned int p_NumItems)
	{
		//std::vector<T*> buffer2(p_NumItems, nullptr);
		T **buffer2 = new T*[p_NumItems];
		
		#ifndef OLDNEW
			Allocator::LinearAllocator oneFrameStack(p_NumItems * sizeof(T));
		#endif

		int j = 0;
		while (j < 5000)
		{
			#ifdef OLDNEW
				delete[] buffer2;
				buffer2 = new T*[p_NumItems];
			#else
				oneFrameStack.clear();
			#endif

			for (unsigned int i = 0; i < p_NumItems; i++)
			{
				buffer2[i] = DBGNEW(oneFrameStack) T;

				if (buffer2[i] != nullptr)
					*buffer2[i] = i + j;
			}

			for (unsigned int i = 1; i < p_NumItems; i++)
			{
				if (buffer2[i] != nullptr)
					*buffer2[i] = *buffer2[i] * *buffer2[i - 1];
				else
					break;
			}

			j++;
		}
	}
};

