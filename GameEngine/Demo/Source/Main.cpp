#include "BaseApp.h"

#include <windows.h>
#include <stdlib.h>

int main(int /*argc*/, char* /*argv*/[])
{
	try
	{
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