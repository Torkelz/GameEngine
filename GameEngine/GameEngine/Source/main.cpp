#include "IToolKit.h"

#include "HighPerformanceTimer.h"

#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <vector>

void functionToBeMeasured()
{
	Allocator::LinearAllocator alloc(1100);
	for (unsigned int i = 0; i < 250; ++i)
	{
		int *k = new(alloc) int;
		*k = i;
	}
}
void functionToBeMeasured2()
{
	for (unsigned int i = 0; i < 250; ++i)
	{
		int *k = new int;
		*k = i;
	}
}
const int THREADS = 16;
const int LOOPS = 1000;

static Allocator::LinearAllocator staticAlloc(THREADS * LOOPS * sizeof(int) * 2);
int* arr[THREADS * LOOPS];
int te = 0;

void threadCallFunction(int id)
{
	for (unsigned int i = 0; i < LOOPS; ++i)
	{
		arr[i + (id * LOOPS)] = new(staticAlloc) int;
		*arr[i + (id * LOOPS)] = i + (id * LOOPS);
	}
}
// 10896 no locks.

int main(int /*argc*/, char* /*argv*/[])
{
	HighPerformanceTimer timer;

	double f1 = timer.measureFunction(std::bind(functionToBeMeasured), 100);
	double f2 = timer.measureFunction(std::bind(functionToBeMeasured2), 100);

	LONGLONG total1, total2;
	total1 = {0};
	total2 = {0};
	for (unsigned int i = 0; i < 100; ++i)
	{
		timer.start();
		functionToBeMeasured();
		total1 += timer.stop();
	}
	for (unsigned int i = 0; i < 100; ++i)
	{
		timer.start();
		functionToBeMeasured2();
		total2 += timer.stop();
	}


	timer.start();

	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < THREADS; ++i)
	{
		threads.push_back(std::thread(threadCallFunction, i));
	}
	//std::cout << "Threads now execute concurrently..." << std::endl;
	for (std::vector<std::thread>::iterator t = threads.begin(); t != threads.end(); ++t)
	{
		t->join();
	}


	std::cout << "Threads completed.\nResults: " << std::to_string(timer.ticksToMs(timer.stop())) << "\n" << std::endl;

	//for (unsigned int i = 0; i < THREADS; ++i)
	//{
	//	for (unsigned int j = 0; j < LOOPS; ++j)
	//	{
	//		std::cout << std::to_string(*arr[j + (i * LOOPS)]) << ",";
	//	}
	//	std::cout << std::endl;
	//}

	int nrErrors = 0;
	for (unsigned int i = 0; i < THREADS*LOOPS; ++i)
	{
		if (*arr[i] != i)
			++nrErrors;
	}
	std::cout << "Errors: " << std::to_string(nrErrors) << std::endl;




	std::cout << std::to_string(f1) << " ms( " << std::to_string(f1 / f2) << " %) " << std::to_string(f2) << std::endl;
	std::cout << std::to_string(total1) << " ticks( " << std::to_string((double)total1 / total2) << " %) " << std::to_string(total2) << std::endl;

	std::system("PAUSE");
	return 0;
}