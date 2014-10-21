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

bool Assignment2::runScenario1(void)
{
	std::thread thread1([&]{ scenario1Thread1(); });
	std::thread thread2([&]{ scenario1Thread2(); });

	thread1.join();
	thread2.join();

	UINT allocated = m_ResourceManager->getAllocated();

	m_TotalSize += texture1->size();
	m_TotalSize += texture2->size();
	m_TotalSize += texture3->size();
	m_TotalSize += texture5->size();
	m_TotalSize += texture6->size();
	m_TotalSize += texture7->size();

	m_ResourceManager->Free(texture1);
	texture1.reset();
	m_ResourceManager->Free(texture2);
	texture2.reset();
	m_ResourceManager->Free(texture3);
	texture3.reset();
	m_ResourceManager->Free(texture4);
	texture4.reset();
	m_ResourceManager->Free(texture5);
	texture5.reset();
	m_ResourceManager->Free(texture6);
	texture6.reset();
	m_ResourceManager->Free(texture7);
	texture7.reset();

	return allocated == m_TotalSize;
}

bool Assignment2::runScenario2(void)
{


	return true;
}

void Assignment2::scenario1Thread1(void)
{
	ResourceZipFile zip = ResourceZipFile();
	zip.initialize(L"..\\Resources\\hibba.zip");
	m_ResourceManager->loadZipLib(&zip, "hibba");

	Resource halo1("halo.jpg", "hibba");
	Resource halo2("halo2.jpg", "hibba");
	Resource halo3("halo3.jpg", "hibba");

	texture1 = m_ResourceManager->getHandle(&halo1);
	texture2 = m_ResourceManager->getHandle(&halo2);
	texture3 = m_ResourceManager->getHandle(&halo3);
}

void Assignment2::scenario1Thread2(void)
{
	ResourceZipFile zip = ResourceZipFile();
	zip.initialize(L"..\\Resources\\hubba.zip");
	m_ResourceManager->loadZipLib(&zip, "hubba");

	Resource halo2("halo2.jpg", "hubba");
	Resource metroid1("metroid.jpg", "hubba");
	Resource metroid2("metroid2.jpg", "hubba");
	Resource metroid3("metroid3.jpg", "hubba");

	texture4 = m_ResourceManager->getHandle(&halo2);
	texture5 = m_ResourceManager->getHandle(&metroid1);
	texture6 = m_ResourceManager->getHandle(&metroid2);
	texture7 = m_ResourceManager->getHandle(&metroid3);
}

void Assignment2::scenario2(void)
{

}