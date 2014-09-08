#include "IToolKit.h"

#include <iostream>
#include <string>

int main(int /*argc*/, char* /*argv*/[])
{
	try{
		Allocator::DoubleFrameAllocator dub(1000);

		for (int i = 0; i < 500000; ++i)
		{
			dub.clearCurrentBuffer();
			for (int i = 0; i < 249; ++i)
			{
				int *k = new(dub) int;

				if (k == nullptr)
					std::cout << "ja e full!\n";
			}
			dub.swap();
		}
	}
	catch (MemoryException e)
	{
		std::cout << e.what() << std::endl;
	}
	std::system("PAUSE");
	return 0;
}