#include "IToolKit.h"
#include "HighPerformanceTimer.h"
#include <iostream>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "Assignment1.h"

int main(int /*argc*/, char* /*argv*/[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Assignment1 ass1;
	HighPerformanceTimer timer;
	std::cout << "Scenario 1: " << std::to_string(timer.measureFunction(std::bind(&Assignment1::scenario1, ass1, 4, 2048), 10)) << std::endl;
	std::cout << "Scenario 2: " << std::to_string(timer.measureFunction(std::bind(&Assignment1::scenario2, ass1, 2048), 10)) << std::endl;
	std::system("PAUSE");
	return 0;
}