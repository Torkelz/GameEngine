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

	std::vector<int> tests;
	tests.push_back(25);
	tests.push_back(50);
	tests.push_back(100);
	tests.push_back(200);
	tests.push_back(400);
	tests.push_back(800);
	tests.push_back(1600);
	tests.push_back(3200);



	std::ofstream myFile(fileName);
	if (myFile.is_open())
	{
		myFile << "X" << ";" << "Time(ms)\n";
		for each(const int &i in tests)
		{
			myFile << std::to_string(i) << ";" << std::to_string(timer.measureFunction(std::bind(&Assignment1::scenario1<int>, ass1, i), 100)) << "\n";
		}
		myFile.close();
	}
}

void scenario2()
{
	Assignment1 ass1;
	HighPerformanceTimer timer;

	std::string fileName;
#ifdef OLDNEW
	fileName = "Scenario2TestsOLDNEW.csv";
#else
	fileName = "Scenario2Tests.csv";
#endif

	std::vector<int> tests;
	tests.push_back(25);
	tests.push_back(50);
	tests.push_back(100);
	tests.push_back(200);
	tests.push_back(400);
	tests.push_back(800);
	tests.push_back(1600);
	tests.push_back(3200);



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