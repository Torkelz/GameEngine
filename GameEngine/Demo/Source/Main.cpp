#include "BaseApp.h"
#include "Logger.h"

#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
	try
	{
		std::ofstream logFile("logFile.txt", std::ofstream::trunc);
		Logger::addOutput(Logger::Level::DEBUG_L, logFile);
		Logger::addOutput(Logger::Level::INFO, std::cout);


		BaseApp app;
		app.init();
		app.run();
		app.shutdown();
	}
	catch (...)
	{
		throw;

		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}