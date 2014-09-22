#pragma once
#include <stdio.h>
#include <map>

typedef std::map<std::string, int> ZipContentsMap;		// maps path to a zip content id

class ZipFile
{
private:
	struct TZipDirHeader;
	struct TZipDirFileHeader;
	struct TZipLocalHeader;

	FILE *m_File;		// Zip file
	char *m_DirData;	// Raw data buffer.
	int  m_NumEntries;	// Number of entries.

	// Pointers to the dir entries in pDirData.
	const TZipDirFileHeader **m_AppDir;

public:
	ZipFile();
	virtual ~ZipFile();

	bool init(const std::wstring &p_ResFileName);
	void end();

	int getNumFiles()const;
	std::string getFilename(int p_I) const;
	int getFileLen(int p_I) const;
	bool readFile(int p_I, void *pBuf);

	// Added to show multi-threaded decompression
	bool readLargeFile(int p_I, void *p_Buffer, void(*progressCallback)(int, bool &));

	int find(const std::string &p_Path) const;

	ZipContentsMap m_ZipContentsMap;
};