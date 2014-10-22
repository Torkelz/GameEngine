#pragma once

#include "IAllocators.h"
#include "HighPerformanceTimer.h"

#include <vector>
#include <thread>
#include <random>
#include <fstream>

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
public:
	bool threadTestPoolAllocator()
	{
		Allocator::PoolAllocator pool(4, 9001);
		std::vector<int*> v1, v2;
		std::thread thread1([&]{ v1 = threadtestPool<int>(pool, 50, 0); });
		std::thread thread2([&]{ v2 = threadtestPool<int>(pool, 50, 1); });

		thread1.join();
		thread2.join();

		std::move(v2.begin(), v2.end(), std::back_inserter(v1));
		for (int i = 0; i < 100; i++)
		{
			if (i != *v1.at(i))
				return false;
		}
		return true;
	}

	bool threadTestStackAllocator()
	{
		Allocator::LinearAllocator stack(9001);
		std::vector<int*> v1, v2;
		std::thread thread1([&]{ v1 = threadtestLinear<int>(stack, 50, 0); });
		std::thread thread2([&]{ v2 = threadtestLinear<int>(stack, 50, 1); });

		thread1.join();
		thread2.join();

		std::move(v2.begin(), v2.end(), std::back_inserter(v1));
		for (int i = 0; i < 100; i++)
		{
			if (i != *v1.at(i))
				return false;
		}
		return true;
	}

	void scenario1Test()
	{
		HighPerformanceTimer timer;

		std::string fileName;
#ifdef OLDNEW
		fileName = "Scenario1TestsOLDNEW.csv";
#else
		fileName = "Scenario1Tests.csv";
#endif
		const int nTests = 32;
		int startValue = 100;
		std::vector<int> tests;
		for (unsigned int i = 0; i < nTests; ++i)
		{
			tests.push_back(startValue);
			startValue += 100;
		}

		std::ofstream myFile(fileName);
		if (myFile.is_open())
		{
			myFile << "X" << ";" << "Time(ms)\n";
			for each(const int &i in tests)
			{
				int* randSeq = new int[1000];
				std::default_random_engine engine;
				std::uniform_int_distribution<int> distribution(0, i - 1);
				engine.seed(9001);

				for (int j = 0; j < 1000; j++)
				{
					randSeq[j] = distribution(engine);
				}

				myFile << std::to_string(i) << ";" << std::to_string(timer.measureFunction(std::bind(&Assignment1::scenario1<int>, this, i, randSeq), 100)) << "\n";

				delete[] randSeq;
			}
			myFile.close();
		}
	}

	void scenario2Test()
	{
		HighPerformanceTimer timer;

		std::string fileName;
#ifdef OLDNEW
		fileName = "Scenario2TestsOLDNEW.csv";
#else
		fileName = "Scenario2Tests.csv";
#endif

		const int nTests = 32;
		int startValue = 100;
		std::vector<int> tests;
		for (unsigned int i = 0; i < nTests; ++i)
		{
			tests.push_back(startValue);
			startValue += 100;
		}

		std::ofstream myFile(fileName);
		if (myFile.is_open())
		{
			myFile << "X" << ";" << "Time(ms)\n";
			for each(const int &i in tests)
			{
				myFile << std::to_string(i) << ";" << std::to_string(timer.measureFunction(std::bind(&Assignment1::scenario2<int>, this, i), 100)) << "\n";
			}
			myFile.close();
		}
	}

	void runAllTests()
	{
		scenario1Test();
		scenario2Test();
		threadTestPoolAllocator();
		threadTestStackAllocator();
	}
private:

	template<typename T = int>
	void scenario1(unsigned int p_NumItems, int* randSeq)
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

		int j = 0;
		int deleted = 0;

		while (j < 5000)
		{
			if (j % 100 == 0)
			{
				DBGDELETE(buffer[randSeq[deleted]], memoryPool);
				++deleted;
			}
			++j;
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
		while (j < 500)
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

	template<typename T = int>
	std::vector<T*> threadtestPool(Allocator::PoolAllocator& pool, unsigned int p_Iterator, int id)
	{
		std::vector<T*> threadmem(p_Iterator, nullptr);
		for (unsigned int i = 0; i < p_Iterator; i++)
		{
			threadmem[i] = DBGNEW(pool) T;

			if (threadmem[i] != nullptr)
				*threadmem[i] = i + p_Iterator * id;
		}

		return threadmem;
	}

	template<typename T = int>
	std::vector<T*> threadtestLinear(Allocator::LinearAllocator& p_stackAllocator, unsigned int p_Iterator, int id)
	{
		std::vector<T*> threadmem(p_Iterator, nullptr);
		for (unsigned int i = 0; i < p_Iterator; i++)
		{
			threadmem[i] = DBGNEW(p_stackAllocator) T;

			if (threadmem[i] != nullptr)
				*threadmem[i] = i + p_Iterator * id;
		}

		return threadmem;
	}

};

