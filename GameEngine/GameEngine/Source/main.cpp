#include "IAllocators.h"
#include "HighPerformanceTimer.h"
#include <iostream>
#include <fstream>


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "Assignment1.h"

void scenario1()
{
	Assignment1 ass1;
	HighPerformanceTimer timer;

	std::string fileName;
#ifdef OLDNEW
	fileName = "Scenario1TestsOLDNEW.csv";
#else
	fileName = "Scenario1Tests.csv";
#endif
	const int nTests = 8;
	int startValue = 25;
	std::vector<int> tests;
	for (unsigned int i = 0; i < nTests; ++i)
	{
		tests.push_back(startValue);
		startValue *= 2;
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

			myFile << std::to_string(i) << ";" << std::to_string(timer.measureFunction(std::bind(&Assignment1::scenario1<int>, ass1, i, randSeq), 100)) << "\n";

			delete[] randSeq;
		}
		myFile.close();
	}
}

void scenario2()
{
	Assignment1 ass1;
	HighPerformanceTimer timer;
	int numberOfItems = 1000;
	int* randSeq = new int[numberOfItems];

	std::string fileName;
#ifdef OLDNEW
	fileName = "Scenario2TestsOLDNEW.csv";
#else
	fileName = "Scenario2Tests.csv";
#endif

	const int nTests = 8;
	int startValue = 25;
	std::vector<int> tests;
	for (unsigned int i = 0; i < nTests; ++i)
	{
		tests.push_back(startValue);
		startValue *= 2;
	}

	std::ofstream myFile(fileName);
	if (myFile.is_open())
	{
		myFile << "X" << ";" << "Time(ms)\n";
		for each(const int &i in tests)
		{
			myFile << std::to_string(i) << ";" << std::to_string(timer.measureFunction(std::bind(&Assignment1::scenario2<int>, ass1, i), 100)) << "\n";
		}
		myFile.close();
	}
}



int main(int /*argc*/, char* /*argv*/[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	scenario1();
	scenario2();

	return 0;
}