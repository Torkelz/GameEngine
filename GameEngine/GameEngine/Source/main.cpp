#include "IToolKit.h"
#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::system("PAUSE");
	return 0;
}