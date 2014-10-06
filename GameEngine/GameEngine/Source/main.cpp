#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "Assignment1.h"
#include <iostream>

#include "IResourceManager.h"

int main(int /*argc*/, char* /*argv*/[])
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Assignment1 ass1;

	//ass1.scenario1Test();
	//ass1.scenario2Test();
	//if (!ass1.threadTestPoolAllocator())
	//	std::cout << "Alligator are not thread safe!" << std::endl;
	//if (!ass1.threadTestStackAllocator())
	//	std::cout << "Stack are not thread safe!" << std::endl;
	//system("pause");
	
	using namespace Res;

	ResourceZipFile *zip = new ResourceZipFile(L"hubba.zip");
	const unsigned int s = 50;
	ResourceManager man(s);
	man.init();
	
	Resource res("scenario1Tests.csv");
	man.loadResource(zip, "hubba");

	std::shared_ptr<ResourceHandle> texture = man.getHandle(&res, "hubba");
	int size = texture->size();


	return EXIT_SUCCESS;
}