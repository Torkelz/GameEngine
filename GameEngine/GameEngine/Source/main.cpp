#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "Assignment1.h"
#include <iostream>

#include "IResourceManager.h"
#define TOTAL_SIZE 200 * 1024 * 1024
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

	Allocator::LinearAllocator allocator(TOTAL_SIZE);
	Allocator::LinearAllocator *resourceAllocator = new(allocator.allocate(16)) Allocator::LinearAllocator();
	char* resourceBuffer = (char*)allocator.allocate(50 * 1024 * 1024);
	resourceAllocator->setBuffer(resourceBuffer);

	UINT zipHeaderSize = sizeof(ResourceZipFile);

	ResourceZipFile *zip = new(allocator.allocate(zipHeaderSize)) ResourceZipFile();
	zip->initialize(L"hubba.zip");
	
	
	//ResourceManager man(resourceAllocator);
	//man.init();
	
	//man.loadResource(zip, "hubba");

	//Resource res("scenario1Tests.csv");

	//std::shared_ptr<ResourceHandle> texture = man.getHandle(&res, "hubba");
	//int size = texture->size();

	zip = nullptr;
	resourceAllocator->clear();
	allocator.clear();

	return EXIT_SUCCESS;
}