#include "ResourceZipFile.h"
#include "ZipFile.h"
#include "Resource.h"
#include "Macros.h"

namespace Res
{
	ResourceZipFile::ResourceZipFile(){}

	ResourceZipFile::~ResourceZipFile()
	{
		SAFE_DELETE(m_ZipFile);
	}

	void ResourceZipFile::initialize(std::wstring p_ResFileName)
	{
		m_ZipFile = nullptr;
		m_ResFileName = p_ResFileName;
	}

	bool ResourceZipFile::open(void)
	{
		m_ZipFile = new ZipFile;
		if (m_ZipFile)
		{
			return m_ZipFile->init(m_ResFileName.c_str());
		}
		return false;
	}

	int ResourceZipFile::getRawResourceSize(const Resource &p_R)
	{
		int resourceNum = m_ZipFile->find(p_R.m_Name.c_str());
		if (resourceNum == -1)
			return -1;

		return m_ZipFile->getFileLen(resourceNum);
	}



	int ResourceZipFile::getRawResource(const Resource &p_R, char *p_Buffer)
	{
		int size = 0;
		int resourceNum = m_ZipFile->find(p_R.m_Name.c_str());
		if (resourceNum >= 0)
		{
			size = m_ZipFile->getFileLen(resourceNum);
			m_ZipFile->readFile(resourceNum, p_Buffer);
		}
		return size;
	}



	int ResourceZipFile::getNumResources(void) const
	{
		return (m_ZipFile == NULL) ? 0 : m_ZipFile->getNumFiles();
	}




	std::string ResourceZipFile::getResourceName(int p_Num) const
	{
		std::string resName = "";
		if (m_ZipFile != NULL && p_Num >= 0 && p_Num < m_ZipFile->getNumFiles())
		{
			resName = m_ZipFile->getFilename(p_Num);
		}
		return resName;
	}

	std::string ResourceZipFile::getFilePath(void) const
	{
		std::string path((const char*)&m_ResFileName[0], sizeof(wchar_t) / sizeof(char)*m_ResFileName.size());
		return path;
	}
}