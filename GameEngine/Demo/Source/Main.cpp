#include "BaseApp.h"
#include "Logger.h"

#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
	std::ofstream logFile("logFile.txt", std::ofstream::trunc);

	Logger::addOutput(Logger::Level::DEBUG_L, logFile);
	Logger::addOutput(Logger::Level::INFO, std::cout);

	try
	{
		BaseApp app;
		app.init();
		app.run();
		app.shutdown();
		Logger::log(Logger::Level::INFO, "Closed the game");
	}
	catch (std::exception& err)
	{
		Logger::log(Logger::Level::FATAL, err.what());
		logFile.close();
		return EXIT_FAILURE;
	}
	catch (...)
	{
		Logger::log(Logger::Level::FATAL, "Unknown exception caught, aborting program");
		logFile.close();
		return EXIT_FAILURE;
	}

	logFile.close();

	return EXIT_SUCCESS;
}