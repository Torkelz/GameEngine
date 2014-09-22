#pragma once
#include <stdio.h>
#include <map>

typedef std::map<std::string, int> ZipContentsMap;		// maps path to a zip content id

namespace Res
{
	class ZipFile
	{
	private:
		struct TZipDirHeader;
		struct TZipDirFileHeader;
		struct TZipLocalHeader;

		FILE *m_File;		// Zip file
		char *m_DirData;	// Raw data buffer.
		int m_NumEntries;	// Number of entries.

		// Pointers to the dir entries in pDirData.
		const TZipDirFileHeader **m_AppDir;

	public:
		ZipFile(void);
		virtual ~ZipFile(void);

		bool init(const std::wstring &p_ResFileName);
		void end(void);

		int getNumFiles(void) const;
		std::string getFilename(int p_Index) const;
		int getFileLen(int p_Index) const;
		bool readFile(int p_Index, void *p_Buffer);

		// Added to show multi-threaded decompression
		bool readLargeFile(int p_Index, void *p_Buffer, void(*progressCallback)(int, bool&));

		int find(const std::string &p_Path) const;

		ZipContentsMap m_ZipContentsMap;
	};
}