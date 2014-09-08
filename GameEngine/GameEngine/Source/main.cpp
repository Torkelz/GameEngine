#include "IToolKit.h"

#include "HighPerformanceTimer.h"

#include <iostream>
#include <functional>
#include <string>

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
	HighPerformanceTimer timer;

	double f1 = timer.measureFunction(std::bind(functionToBeMeasured), 100);
	double f2 = timer.measureFunction(std::bind(functionToBeMeasured2), 100);

	std::cout << std::to_string(f1) << " ? " << std::to_string(f2) << std::endl;
	std::system("PAUSE");
	return 0;
}