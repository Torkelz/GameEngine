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
static Allocator::LinearAllocator staticAlloc(5100);
std::vector<int*> list;

void threadCallFunction(int id)
{
	for (unsigned int i = 0; i < 40; ++i)
	{
		list.push_back(new(staticAlloc) int);
		*list.back() = i+(id*40);
	}
}


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

	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < 4; ++i)
	{
		threads.push_back(std::thread(threadCallFunction, i));
	}
	std::cout << "Threads now execute concurrently..." << std::endl;
	for (std::vector<std::thread>::iterator t = threads.begin(); t != threads.end(); ++t)
	{
		t->join();
	}
	std::cout << "Threads completed.\nResults: \n" << std::endl;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 40; ++j)
		{
			std::cout << std::to_string(*list.at((40 * i) + j)) << ",";
		}
		std::cout << std::endl;
	}


	std::cout << std::to_string(f1) << " ms( " << std::to_string(f1 / f2) << " %) " << std::to_string(f2) << std::endl;
	std::cout << std::to_string(total1) << " ticks( " << std::to_string((double)total1 / total2) << " %) " << std::to_string(total2) << std::endl;

	std::system("PAUSE");
	return 0;
}