#pragma once

#include "IAllocators.h"
#include <vector>
#include <thread>

#define OLDNEW

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
	void scenario1(unsigned int p_NumItems)
	{
		std::vector<T*> buffer(p_NumItems, nullptr);

	#ifndef OLDNEW
		Allocator::PoolAllocator memoryPool(sizeof(T), p_NumItems);
	#endif

		for (unsigned int i = 0; i < p_NumItems; i++)
		{
			buffer[i] = DBGNEW(memoryPool) T;
			if (buffer[i] != nullptr)
				*buffer[i] = i;
		}

		unsigned int removed = 0;
		int j = 0;
		while (j < 5000)
		{
			if (j % 100 == 0)
			{
				DBGDELETE(buffer[removed], memoryPool);
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
			DBGDELETE(buffer[i], nullptr);
		}
#endif
	}

	template<typename T>
	void scenario2(unsigned int p_NumItems)
	{

	#ifndef OLDNEW
		Allocator::LinearAllocator oneFrameStack(p_NumItems * sizeof(T));
	#endif
		
		std::vector<T*> buffer(p_NumItems, nullptr);
		int j = 0;
		while (j < 5000)
		{

		#ifdef OLDNEW
			for (unsigned int i = 0; i < p_NumItems; i++)
			{
				DBGDELETE(buffer[i], nullptr);
			}
		#else
			oneFrameStack.clear();
		#endif

			for (unsigned int i = 0; i < p_NumItems; i++)
			{
				buffer[i] = DBGNEW(oneFrameStack) T;

				if (buffer[i] != nullptr)
					*buffer[i] = i + j;
			}
			j++;
		}

#ifdef OLDNEW
		for (unsigned int i = 0; i < p_NumItems; i++)
		{
			DBGDELETE(buffer[i], nullptr);
		}
#else
		oneFrameStack.clear();
#endif;
	}

	//template<typename T>
	//void threadtest(Allocator::PoolAllocator& pool, unsigned int p_Iterator)
	//{
	//	std::vector<T*> threadmem(p_Iterator, nullptr);
	//	for (int i = 0; i < p_Iterator; i++)
	//	{
	//		threadmem[i] = DBGNEW(pool) T;

	//		if (threadmem[i] != nullptr)
	//			*threadmem[i] = i + 2000;
	//	}
	//}

	//void threadTestPoolAllocator()
	//{
	//	Allocator::PoolAllocator pool(4, 9001);

	//	std::thread thread1(&Assignment1::threadtest<int>, *this, std::ref(pool), 50);
	//	std::thread thread2(&Assignment1::threadtest<int>, *this, std::ref(pool), 50);

	//	thread1.join();
	//	thread2.join();
	//}
};

