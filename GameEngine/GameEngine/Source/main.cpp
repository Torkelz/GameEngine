#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "Assignment1.h"
#include "Assignment2.h"
#include <iostream>

#include "IResourceManager.h"
#define TOTAL_SIZE 200 * 1024 * 1024
int main(int /*argc*/, char* /*argv*/[])
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Allocator::LinearAllocator alloc(1024);

	std::vector<short*> buffer(128, nullptr);

	for (unsigned int i = 0; i < buffer.size(); i++)
	{
		buffer.at(i) = alloc.allocate<short>(8);
	}
	int lol = 0;
	//Assignment1 ass1;

	//OLD STUFF
	//ass1.scenario1Test();
	//ass1.scenario2Test();
	//if (!ass1.threadTestPoolAllocator())
	//	std::cout << "Alligator are not thread safe!" << std::endl;
	//if (!ass1.threadTestStackAllocator())
	//	std::cout << "Stack are not thread safe!" << std::endl;
	//system("pause");
	
	//using namespace Res;

	//ResourceZipFile zip = ResourceZipFile();
	//zip.initialize(L"..\\Resources\\hubba.zip");
	//
	//ResourceManager man(TOTAL_SIZE);

	//man.init();
	//man.loadResource(&zip, "hubba");

	//man.registerLoader(std::shared_ptr<IResourceLoader>(new OBJResourceLoader()));
	//Resource res("assignment1.xlsx", "hubba");
	//
	////Resource re("hubba\\models\\Street Light\\street_lamp.obj", "hubba3");

	//std::weak_ptr<ResourceHandle> texture = man.getHandle(&res);
	//int size = texture.lock()->size();

	//man.Free(texture.lock());
	//*/
	//
	//Assignment2 scenario1;
	//scenario1.initialize(TOTAL_SIZE);

	//int result = EXIT_SUCCESS;
	//if (!scenario1.runScenario1())
	//{
	//	result = EXIT_FAILURE;
	//}

	//if (!scenario1.runScenario2())
	//{
	//	result += EXIT_FAILURE + 1;
	//}


	return 0;
}