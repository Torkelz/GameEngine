#include <cctype>			// for std::tolower

#include "ZipFile.h"
#include "Macros.h"

#include "zlib.h"
#include <string.h>
#include <algorithm>

namespace Res
{
	// --------------------------------------------------------------------------
	// Basic types.
	// --------------------------------------------------------------------------
	typedef unsigned long dword;
	typedef unsigned short word;
	typedef unsigned char byte;

	// --------------------------------------------------------------------------
	// ZIP file structures. Note these have to be packed.
	// --------------------------------------------------------------------------

#pragma pack(1)
	struct ZipFile::TZipLocalHeader
	{
		enum
		{
			SIGNATURE = 0x04034b50,
		};
		dword   sig;
		word    version;
		word    flag;
		word    compression;      // Z_NO_COMPRESSION or Z_DEFLATED
		word    modTime;
		word    modDate;
		dword   crc32;
		dword   cSize;
		dword   ucSize;
		word    fnameLen;         // Filename string follows header.
		word    xtraLen;          // Extra field follows filename.
	};

	struct ZipFile::TZipDirHeader
	{
		enum
		{
			SIGNATURE = 0x06054b50
		};
		dword   sig;
		word    nDisk;
		word    nStartDisk;
		word    nDirEntries;
		word    totalDirEntries;
		dword   dirSize;
		dword   dirOffset;
		word    cmntLen;
	};

	struct ZipFile::TZipDirFileHeader
	{
		enum
		{
			SIGNATURE = 0x02014b50
		};
		dword   sig;
		word    verMade;
		word    verNeeded;
		word    flag;
		word    compression;      // COMP_xxxx
		word    modTime;
		word    modDate;
		dword   crc32;
		dword   cSize;            // Compressed size
		dword   ucSize;           // Uncompressed size
		word    fnameLen;         // Filename string follows header.
		word    xtraLen;          // Extra field follows filename.
		word    cmntLen;          // Comment field follows extra field.
		word    diskStart;
		word    intAttr;
		dword   extAttr;
		dword   hdrOffset;

		char *GetName() const { return (char *)(this + 1); }
		char *GetExtra() const { return GetName() + fnameLen; }
		char *GetComment() const { return GetExtra() + xtraLen; }
	};

#pragma pack()

	ZipFile::ZipFile(void) :
		m_NumEntries(0), m_File(nullptr), m_DirData(nullptr)
	{
	}

	ZipFile::~ZipFile(void)
	{
		end();
		if (m_File)
			fclose(m_File);
	}

	bool ZipFile::init(const std::wstring &p_ResFileName)
	{
		end();

		_wfopen_s(&m_File, p_ResFileName.c_str(), _T("rb"));
		if (!m_File)
			return false;

		// Assuming no extra comment at the end, read the whole end record.
		TZipDirHeader dh;

		fseek(m_File, -(int)sizeof(dh), SEEK_END);
		long dhOffset = ftell(m_File);
		memset(&dh, 0, sizeof(dh));
		fread(&dh, sizeof(dh), 1, m_File);

		// Check
		if (dh.sig != TZipDirHeader::SIGNATURE)
			return false;

		// Go to the beginning of the directory.
		fseek(m_File, dhOffset - dh.dirSize, SEEK_SET);

		// Allocate the data buffer, and read the whole thing.
		m_DirData = new char[dh.dirSize + dh.nDirEntries*sizeof(*m_AppDir)];
		if (!m_DirData)
			return false;
		memset(m_DirData, 0, dh.dirSize + dh.nDirEntries*sizeof(*m_AppDir));
		fread(m_DirData, dh.dirSize, 1, m_File);

		// Now process each entry.
		char *pfh = m_DirData;
		m_AppDir = (const TZipDirFileHeader **)(m_DirData + dh.dirSize);

		bool success = true;

		for (int i = 0; i < dh.nDirEntries && success; i++)
		{
			TZipDirFileHeader &fh = *(TZipDirFileHeader*)pfh;

			// Store the address of nth file for quicker access.
			m_AppDir[i] = &fh;

			// Check the directory entry integrity.
			if (fh.sig != TZipDirFileHeader::SIGNATURE)
				success = false;
			else
			{
				pfh += sizeof(fh);

				// Convert UNIX slashes to DOS backlashes.
				for (int j = 0; j < fh.fnameLen; j++)
				if (pfh[j] == '/')
					pfh[j] = '\\';

				char fileName[_MAX_PATH];
				memcpy(fileName, pfh, fh.fnameLen);
				fileName[fh.fnameLen] = 0;
				_strlwr_s(fileName, _MAX_PATH);
				std::string spath = fileName;
				m_ZipContentsMap[spath] = i;

				// Skip name, extra and comment fields.
				pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
			}
		}
		if (!success)
		{
			SAFE_DELETE_ARRAY(m_DirData);
		}
		else
		{
			m_NumEntries = dh.nDirEntries;
		}

		return success;
	}

	int ZipFile::find(const std::string &p_Path) const
	{
		std::string lowerCase = p_Path;
		std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
		ZipContentsMap::const_iterator i = m_ZipContentsMap.find(lowerCase);
		if (i == m_ZipContentsMap.end())
			return -1;

		return i->second;
	}

	void ZipFile::end(void)
	{
		m_ZipContentsMap.clear();
		SAFE_DELETE_ARRAY(m_DirData);
		m_NumEntries = 0;
	}

	int ZipFile::getNumFiles(void) const
	{
		return m_NumEntries;
	}

	std::string ZipFile::getFilename(int p_Index)  const
	{
		std::string fileName = "";
		if (p_Index >= 0 && p_Index < m_NumEntries)
		{
			char pszDest[_MAX_PATH];
			memcpy(pszDest, m_AppDir[p_Index]->GetName(), m_AppDir[p_Index]->fnameLen);
			pszDest[m_AppDir[p_Index]->fnameLen] = '\0';
			fileName = pszDest;
		}
		return fileName;
	}

	int ZipFile::getFileLen(int p_Index) const
	{
		if (p_Index < 0 || p_Index >= m_NumEntries)
			return -1;
		else
			return m_AppDir[p_Index]->ucSize;
	}

	bool ZipFile::readFile(int p_Index, void *p_Buffer)
	{
		if (p_Buffer == NULL || p_Index < 0 || p_Index >= m_NumEntries)
			return false;

		// Quick'n dirty read, the whole file at once.
		// Ungood if the ZIP has huge files inside

		// Go to the actual file and read the local header.
		fseek(m_File, m_AppDir[p_Index]->hdrOffset, SEEK_SET);
		TZipLocalHeader h;

		memset(&h, 0, sizeof(h));
		fread(&h, sizeof(h), 1, m_File);
		if (h.sig != TZipLocalHeader::SIGNATURE)
			return false;

		// Skip extra fields
		fseek(m_File, h.fnameLen + h.xtraLen, SEEK_CUR);

		if (h.compression == Z_NO_COMPRESSION)
		{
			// Simply read in raw stored data.
			fread(p_Buffer, h.cSize, 1, m_File);
			return true;
		}
		else if (h.compression != Z_DEFLATED)
			return false;

		// Alloc compressed data buffer and read the whole stream
		char *pcData = new char[h.cSize];
		if (!pcData)
			return false;

		memset(pcData, 0, h.cSize);
		fread(pcData, h.cSize, 1, m_File);

		bool ret = true;

		// Setup the inflate stream.
		z_stream stream;
		int err;

		stream.next_in = (Bytef*)pcData;
		stream.avail_in = (uInt)h.cSize;
		stream.next_out = (Bytef*)p_Buffer;
		stream.avail_out = h.ucSize;
		stream.zalloc = (alloc_func)0;
		stream.zfree = (free_func)0;

		// Perform inflation. wbits < 0 indicates no zlib header inside the data.
		err = inflateInit2(&stream, -MAX_WBITS);
		if (err == Z_OK)
		{
			err = inflate(&stream, Z_FINISH);
			inflateEnd(&stream);
			if (err == Z_STREAM_END)
				err = Z_OK;
			inflateEnd(&stream);
		}
		if (err != Z_OK)
			ret = false;

		delete[] pcData;
		return ret;
	}

	bool ZipFile::readLargeFile(int p_Index, void *p_Buffer, void(*progressCallback)(int, bool&))
	{
		if (p_Buffer == NULL || p_Index < 0 || p_Index >= m_NumEntries)
			return false;

		// Quick'n dirty read, the whole file at once.
		// Ungood if the ZIP has huge files inside

		// Go to the actual file and read the local header.
		fseek(m_File, m_AppDir[p_Index]->hdrOffset, SEEK_SET);
		TZipLocalHeader h;

		memset(&h, 0, sizeof(h));
		fread(&h, sizeof(h), 1, m_File);
		if (h.sig != TZipLocalHeader::SIGNATURE)
			return false;

		// Skip extra fields
		fseek(m_File, h.fnameLen + h.xtraLen, SEEK_CUR);

		if (h.compression == Z_NO_COMPRESSION)
		{
			// Simply read in raw stored data.
			fread(p_Buffer, h.cSize, 1, m_File);
			return true;
		}
		else if (h.compression != Z_DEFLATED)
			return false;

		// Alloc compressed data buffer and read the whole stream
		char *pcData = new char[h.cSize];
		if (!pcData)
			return false;

		memset(pcData, 0, h.cSize);
		fread(pcData, h.cSize, 1, m_File);

		bool ret = true;

		// Setup the inflate stream.
		z_stream stream;
		int err;

		stream.next_in = (Bytef*)pcData;
		stream.avail_in = (uInt)h.cSize;
		stream.next_out = (Bytef*)p_Buffer;
		stream.avail_out = (128 * 1024); //  read 128k at a time h.ucSize;
		stream.zalloc = (alloc_func)0;
		stream.zfree = (free_func)0;

		// Perform inflation. wbits < 0 indicates no zlib header inside the data.
		err = inflateInit2(&stream, -MAX_WBITS);
		if (err == Z_OK)
		{
			uInt count = 0;
			bool cancel = false;
			while (stream.total_in < (uInt)h.cSize && !cancel)
			{
				err = inflate(&stream, Z_SYNC_FLUSH);
				if (err == Z_STREAM_END)
				{
					err = Z_OK;
					break;
				}
				else if (err != Z_OK)
				{
					//GCC_ASSERT(0 && "Something happened.");
					break;
				}

				stream.avail_out = (128 * 1024);
				stream.next_out += stream.total_out;

				progressCallback(count * 100 / h.cSize, cancel);
			}
			inflateEnd(&stream);
		}
		if (err != Z_OK)
			ret = false;

		delete[] pcData;
		return ret;
	}
}