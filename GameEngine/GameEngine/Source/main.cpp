#include "IToolKit.h"
#include "Assignment1.h"
#include "HighPerformanceTimer.h"
#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


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

int main(int /*argc*/, char* /*argv*/[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Assignment1 ass1;
	HighPerformanceTimer timer;
	using std::placeholders::_1;
	using std::placeholders::_2;
	double test1 = timer.measureFunction(std::bind(&Assignment1::scenario1, ass1, 4, 2048), 1000);
	LONGLONG total1 = { 0 };
	for (int i = 0; i < 1000; i++)
	{
		timer.start();
		ass1.scenario1(4, 2049);
		total1 +=timer.stop();
	}
	std::cout << std::to_string(test1) << std::endl;
	//ass1.scenario2(2048);
	std::system("PAUSE");
	return 0;
}