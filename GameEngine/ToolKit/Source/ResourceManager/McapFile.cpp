#include <cctype>			// for std::tolower
#include <sstream>
#include <fstream>

#include "Macros.h"
#include "McapFile.h"

namespace Res
{
	struct McapFile::McapDirFileHeader
	{
		int start;
		int size;
	};

	McapFile::McapFile(void) :
		m_NumEntries(0), m_File(nullptr){}

	McapFile::~McapFile(void)
	{
		end();
		if (m_File)
			fclose(m_File);
	}

	bool McapFile::init(const std::wstring &p_ResFileName)
	{
		end();

		std::fstream fileStream(p_ResFileName);

		if (!fileStream.is_open())
			return false;

		std::string line;
		int index = 0;
		int headerSize = 0;
		try{
			while (std::getline(fileStream, line))
			{
				if (line == "# Header")
				{
					continue;
				}
				else if (line == "# End of Header")
				{
					headerSize = fileStream.tellg();
					break;
				}

				McapDirFileHeader header;
				std::string name;

				std::stringstream stream;
				stream << line;
				stream >> name >> header.start >> header.size;

				m_McapContentsMap.insert(std::make_pair(name, index++));
				m_AppDir.push_back(header);
			}
			fileStream.close();
			m_NumEntries = m_AppDir.size();
			_wfopen_s(&m_File, p_ResFileName.c_str(), _T("rb"));

			for (McapDirFileHeader &h : m_AppDir)
			{
				h.start += headerSize;
			}
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	void McapFile::end(void)
	{
		m_McapContentsMap.clear();
		m_AppDir.clear();
		m_NumEntries = 0;
	}

	int McapFile::getNumFiles(void) const
	{
		return m_McapContentsMap.size();
	}

	std::string McapFile::getFilename(int p_Index) const
	{
		for (auto &a : m_McapContentsMap)
		{
			if (a.second == p_Index)
				return a.first;
		}
	}

	int McapFile::getFileLen(int p_Index) const
	{
		return m_AppDir.at(p_Index).size;
	}

	bool McapFile::readFile(int p_Index, void *p_Buffer)
	{
		McapDirFileHeader header;
		header = m_AppDir.at(p_Index);

		// Go to the actual file and read the local header.

		if (fseek(m_File, m_AppDir[p_Index].start, SEEK_SET) != 0)
			return false;

		if (fread(p_Buffer, m_AppDir[p_Index].size, 1, m_File) != 1)
			return false;

		return true;
	}

	// Added to show multi-threaded decompression
	bool McapFile::readLargeFile(int p_Index, void *p_Buffer, void(*progressCallback)(int, bool&))
	{
		return false;
	}

	int McapFile::find(const std::string &p_Path) const
	{
		return m_McapContentsMap.at(p_Path);
	}
}