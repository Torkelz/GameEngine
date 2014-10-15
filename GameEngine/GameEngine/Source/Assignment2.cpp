#include "Assignment2.h"

#include <thread>

using namespace Res;

Assignment2::Assignment2(void)
{
	m_ResourceManager = nullptr;
	m_TotalSize = 0;
}

Assignment2::~Assignment2(void)
{
	delete m_ResourceManager;
	m_ResourceManager = nullptr;

}

void Assignment2::initialize(UINT p_CacheSize)
{
	m_ResourceManager = new ResourceManager(p_CacheSize);
	m_ResourceManager->init();
}

bool Assignment2::runTest(void)
{
	std::thread thread1([&]{allocThread1(); });
	std::thread thread2([&]{allocThread2(); });

	thread1.join();
	thread2.join();

	UINT allocated = 0;// m_ResourceManager->getAllocated();

	m_TotalSize += texture1.lock()->size();
	m_TotalSize += texture2.lock()->size();
	m_TotalSize += texture3.lock()->size();
	m_TotalSize += texture4.lock()->size();
	m_TotalSize += texture5.lock()->size();
	m_TotalSize += texture6.lock()->size();
	m_TotalSize += texture7.lock()->size();

	return allocated == m_TotalSize;
}

void Assignment2::allocThread1()
{
	ResourceZipFile zip = ResourceZipFile();
	zip.initialize(L"..\\Resources\\hibba.zip");
	m_ResourceManager->loadResource(&zip, "halo");

	Resource halo1("halo.jpg", "hibba");
	Resource halo2("halo2.jpg", "hibba");
	Resource halo3("halo3.jpg", "hibba");

	texture1 = m_ResourceManager->getHandle(&halo1);
	texture2 = m_ResourceManager->getHandle(&halo2);
	texture3 = m_ResourceManager->getHandle(&halo3);

}

void Assignment2::allocThread2()
{
	ResourceZipFile zip = ResourceZipFile();
	zip.initialize(L"..\\Resources\\hubba.zip");
	m_ResourceManager->loadResource(&zip, "metroid");

	Resource assignment("assignment1.xlsx", "hubba");
	Resource metroid1("metroid.jpg", "hubba");
	Resource metroid2("metroid2.jpg", "hubba");
	Resource metroid3("metroid3.jpg", "hubba");

	texture4 = m_ResourceManager->getHandle(&assignment);
	texture5 = m_ResourceManager->getHandle(&metroid1);
	texture6 = m_ResourceManager->getHandle(&metroid2);
	texture7 = m_ResourceManager->getHandle(&metroid3);
}