#pragma once
#include "IResourceManager.h"

class Assignment2
{
public:
	typedef unsigned int UINT;
private: 
	Res::ResourceManager *m_ResourceManager;
	std::shared_ptr<Res::ResourceHandle> texture1;
	std::shared_ptr<Res::ResourceHandle> texture2;
	std::shared_ptr<Res::ResourceHandle> texture3;
	std::shared_ptr<Res::ResourceHandle> texture4;
	std::shared_ptr<Res::ResourceHandle> texture5;
	std::shared_ptr<Res::ResourceHandle> texture6;
	std::shared_ptr<Res::ResourceHandle> texture7;
	int m_TotalSize;

public:
	Assignment2(void);
	~Assignment2(void);

	void initialize(UINT p_CacheSize);
	bool runScenario1(void);
	bool runScenario2(void);

private:
	void scenario1Thread1(void);
	void scenario1Thread2(void);
	void scenario2(void);
};