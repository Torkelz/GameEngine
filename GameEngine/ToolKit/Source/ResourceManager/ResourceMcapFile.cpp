#include "ResourceMcapFile.h"
#include "Macros.h"
#include "Resource.h"

namespace Res
{
	ResourceMcapFile::ResourceMcapFile() :
		m_McapFile(nullptr){}

	ResourceMcapFile::~ResourceMcapFile(void)
	{
		SAFE_DELETE(m_McapFile);
	}

	void ResourceMcapFile::initialize(std::wstring p_ResFileName)
	{
		m_McapFile = nullptr;
		m_ResFileName = p_ResFileName;
	}

	bool ResourceMcapFile::open(void)
	{
		m_McapFile = new McapFile;
		if (m_McapFile)
		{
			return m_McapFile->init(m_ResFileName.c_str());
		}
		return false;
	}

	int ResourceMcapFile::getRawResourceSize(const Resource &p_R)
	{
		int resourceNum = m_McapFile->find(p_R.m_Name.c_str());
		if (resourceNum == -1)
			return -1;

		return m_McapFile->getFileLen(resourceNum);
	}

	int ResourceMcapFile::getRawResource(const Resource &p_R, char *p_Buffer)
	{
		int size = 0;
		int resourceNum = m_McapFile->find(p_R.m_Name.c_str());
		if (resourceNum >= 0)
		{
			size = m_McapFile->getFileLen(resourceNum);
			m_McapFile->readFile(resourceNum, p_Buffer);
		}
		return size;
	}

	int ResourceMcapFile::getNumResources(void) const
	{
		return (m_McapFile == NULL) ? 0 : m_McapFile->getNumFiles();
	}

	std::string ResourceMcapFile::getResourceName(int p_Num) const
	{
		std::string resName = "";
		if (m_McapFile != NULL && p_Num >= 0 && p_Num < m_McapFile->getNumFiles())
		{
			resName = m_McapFile->getFilename(p_Num);
		}
		return resName;
	}

	std::string ResourceMcapFile::getFilePath(void) const
	{
		std::string path((const char*)&m_ResFileName[0], sizeof(wchar_t) / sizeof(char)*m_ResFileName.size());
		return path;
	}
}