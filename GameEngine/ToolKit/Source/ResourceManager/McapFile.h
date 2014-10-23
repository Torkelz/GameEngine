#pragma once
#include <stdio.h>
#include <map>
#include <vector>

typedef std::map<std::string, int> McapContentsMap;		// maps path to a mcap content id

namespace Res
{
	class McapFile
	{
	private:
		struct McapDirFileHeader;

		FILE *m_File;		// Zip file
		char *m_DirData;	// Raw data buffer.
		int m_NumEntries;	// Number of entries.

		// Pointers to the dir entries in pDirData.
		std::vector<const McapDirFileHeader> m_AppDir;

	public:
		McapFile(void);
		virtual ~McapFile(void);

		bool init(const std::wstring &p_ResFileName);
		void end(void);

		int getNumFiles(void) const;
		std::string getFilename(int p_Index) const;
		int getFileLen(int p_Index) const;
		bool readFile(int p_Index, void *p_Buffer);

		// Added to show multi-threaded decompression
		bool readLargeFile(int p_Index, void *p_Buffer, void(*progressCallback)(int, bool&));

		int find(const std::string &p_Path) const;

		McapContentsMap m_McapContentsMap;
	};
}