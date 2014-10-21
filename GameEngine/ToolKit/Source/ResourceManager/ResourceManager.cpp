#include "ResourceManager.h"
#include "IResourceFile.h"
#include "IResourceLoader.h"
#include "DefaultResourceLoader.h"
#include "Resource.h"
#include "ResourceHandle.h"
#include "Macros.h"

#include "../Logger/Logger.h"
#include <algorithm>
#include <cctype>

#include <fstream>

namespace Res
{
	ResourceManager::ResourceManager(UINT p_CacheSize) :
		m_CacheSize(p_CacheSize)
	{ }

	ResourceManager::~ResourceManager(void)
	{
		while (!m_Lru.empty())
		{
			freeOneResource();
		}
	}

	ResourceManager::UINT ResourceManager::getAllocated(void)
	{
		return m_Allocated;
	}

	void ResourceManager::init(void)
	{
		registerLoader(std::shared_ptr<IResourceLoader>(new DefaultResourceLoader()));
	}

	bool ResourceManager::loadZipLib(IResourceFile *p_Resource, std::string p_ZipLibr)
	{
		if (p_Resource->open())
		{
			m_HandleLock.lock();
			m_FileMap.insert(std::pair<std::string, IResourceFile*>(p_ZipLibr, p_Resource));
			std::ifstream fileStream;
			std::string guidName = std::string("..\\Resources\\" + p_ZipLibr + ".guid");
			
			fileStream.open(guidName.c_str());
			{
				std::string line;
				std::string guid;
				std::string internalPath;
				fileStream >> guid >> internalPath;
				internalPath = std::string(p_ZipLibr + "/" + internalPath);
				while (std::getline(fileStream, line))
				{
					m_GUID_Table.insert(std::pair<std::string, UINT>(internalPath, std::atoi(guid.c_str())));
					fileStream >> guid >> internalPath;
					internalPath = std::string(p_ZipLibr + "/" + internalPath);
				}
				m_GUID_Table.insert(std::pair<std::string, UINT>(internalPath, std::atoi(guid.c_str())));
			}
			m_HandleLock.unlock();
			return true;
		}

		return false;
	}

	void ResourceManager::registerLoader(std::shared_ptr<IResourceLoader> p_Loader)
	{
		m_ResourceLoaders.push_front(p_Loader);
	}

	std::shared_ptr<ResourceHandle> ResourceManager::getHandle(Resource *p_R)
	{
		m_HandleLock.lock();
		std::shared_ptr<ResourceHandle> handle(find(p_R));
		if (handle == nullptr)
		{
			handle = load(p_R);
		}
		else
		{
			update(handle);
		}
		m_HandleLock.unlock();
		return handle;
	}

	std::shared_ptr<ResourceHandle> ResourceManager::load(Resource *p_R)
	{
		// Create a new resource and add it to the lru list and map
		std::shared_ptr<IResourceLoader> loader;
		std::shared_ptr<ResourceHandle> handle;

		for (ResourceLoaders::iterator it = m_ResourceLoaders.begin(); it != m_ResourceLoaders.end(); ++it)
		{
			std::shared_ptr<IResourceLoader> testLoader = *it;

			if (wildcardMatch(testLoader->getPattern().c_str(), p_R->m_Name.c_str()))
			{
				loader = testLoader;
				break;
			}
		}

		if (!loader)
		{
			return handle;		// Resource not loaded!
		}

		IResourceFile *file = nullptr;
		for each (auto &res in m_FileMap)
		{
			if (res.first == p_R->m_ZipName)
			{
				file = res.second;
			}
		}
		if (!file)
		{
			Logger::log(Logger::Level::WARNING, "File not found in resource file map: " + p_R->m_Name);
			return std::shared_ptr<ResourceHandle>();
		}

		int rawSize = file->getRawResourceSize(*p_R);
		if (rawSize < 0)
		{
			Logger::log(Logger::Level::WARNING, "File size is 0 or less: " + p_R->m_Name);
			return std::shared_ptr<ResourceHandle>();
		}

		int allocSize = rawSize + ((loader->addNullZero()) ? (1) : (0));
		char *rawBuffer = loader->useRawFile() ? Allocate(allocSize) : new char[allocSize];
		memset(rawBuffer, 0, allocSize);

		if (!rawBuffer || file->getRawResource(*p_R, rawBuffer) == 0)
		{
			Logger::log(Logger::Level::WARNING, "Resource cache is out of memory! Dumping memory state.");
			
			int resourceNum = 1;
			for (auto &res : m_Lru)
			{
				Logger::log(Logger::Level::WARNING, "File " + std::to_string(resourceNum) + ", File name: " + res->getName());
				++resourceNum;
			}

			Logger::log(Logger::Level::WARNING, "Tried to load file: " + p_R->m_Name);

			// resource cache out of memory
			return std::shared_ptr<ResourceHandle>();
		}

		char *buffer = nullptr;
		unsigned int size = 0;

		if (loader->useRawFile())
		{
			buffer = rawBuffer;
			handle = std::shared_ptr<ResourceHandle>(new ResourceHandle(*p_R, buffer, rawSize, this));
		}
		else
		{
			size = loader->getLoadedResourceSize(rawBuffer, rawSize);
			buffer = Allocate(size);
			if (!rawBuffer || !buffer)
			{
				// resource cache out of memory
				return std::shared_ptr<ResourceHandle>();
			}
			handle = std::shared_ptr<ResourceHandle>(new ResourceHandle(*p_R, buffer, size, this));
			bool success = loader->loadResource(rawBuffer, rawSize, handle);

			if (loader->discardRawBufferAfterLoad())
			{
				SAFE_DELETE_ARRAY(rawBuffer);
			}

			if (!success)
			{
				// resource cache out of memory
				return std::shared_ptr<ResourceHandle>();
			}
		}

		if (handle)
		{
			m_Lru.push_front(handle);
			m_Resources[p_R->m_Name] = handle;

			std::map<std::string, UINT>::iterator it_all = m_GUID_Table.find(p_R->m_ZipName + "/" + p_R->m_Name);
			if (it_all == m_GUID_Table.end())
			{
				Logger::log(Logger::Level::FATAL, "Can't find GUID for file " + p_R->m_Name + "in archive " + p_R->m_ZipName + "."); 
			}

			UINT guid = it_all->second;
			m_LoadedResources.insert(std::pair<UINT, std::string>(guid, p_R->m_Name));
		}

		return handle;
	}

	std::shared_ptr<ResourceHandle> ResourceManager::find(Resource *p_R)
	{
		std::map<std::string, UINT>::iterator it_all = m_GUID_Table.find(p_R->m_ZipName + "/" + p_R->m_Name);
		if (it_all == m_GUID_Table.end())
		{
			return std::shared_ptr<ResourceHandle>();
		}
		UINT guid = it_all->second;

		std::map<UINT, std::string>::iterator it_loaded = m_LoadedResources.find(guid);
		if (it_loaded == m_LoadedResources.end())
		{
			return std::shared_ptr<ResourceHandle>();
		}
		std::string path = it_loaded->second;
		std::size_t pos = path.find("/");
		std::string internalPath = path.substr(pos + 1);


		ResHandleMap::iterator i = m_Resources.find(internalPath);
		if (i == m_Resources.end())
		{
			return std::shared_ptr<ResourceHandle>();
		}

		return i->second;
	}

	void ResourceManager::update(std::shared_ptr<ResourceHandle> p_Handle)
	{
		m_Lru.remove(p_Handle);
		m_Lru.push_front(p_Handle);
	}

	char *ResourceManager::Allocate(UINT p_Size)
	{
		if (!makeRoom(p_Size))
			return nullptr;

		char *mem = new char[p_Size];
		if (mem)
		{
			m_Allocated += p_Size;
		}

		return mem;
	}

	void ResourceManager::freeOneResource(void)
	{
		ResHandleList::iterator gonner = m_Lru.end();
		gonner--;

		std::shared_ptr<ResourceHandle> handle = *gonner;
		m_Lru.pop_back();

		m_Resources.erase(handle->m_Resource.m_Name);

		std::string zipPathName = std::string(handle->m_Resource.m_ZipName + "/" + handle->m_Resource.m_Name);

		handle.reset();
		// Note - you can't change the resource cache size yet - the resource bits could still actually be
		// used by some sybsystem holding onto the ResourceHandle. Only when it goes out of scope can the memory
		// be actually free again.
	}

	void ResourceManager::flush(void)
	{
		while (!m_Lru.empty())
		{
			std::shared_ptr<ResourceHandle> handle = *(m_Lru.begin());
			Free(handle);
			m_Lru.pop_front();
		}
	}

	bool ResourceManager::makeRoom(UINT p_Size)
	{
		if (p_Size > m_CacheSize)
		{
			return false;
		}

		// return null if there's no possible way to allocate the memory
		while (p_Size > (m_CacheSize - m_Allocated))
		{
			// The cache is empty, and there's still not enough room.
			if (m_Lru.empty())
			{
				return false;
			}

			freeOneResource();
		}
		return true;
	}
	
	void ResourceManager::Free(std::shared_ptr<ResourceHandle> p_Gonner)
	{
		m_HandleLock.lock();
		m_Lru.remove(p_Gonner);
		m_Resources.erase(p_Gonner->m_Resource.m_Name);
		// Note - the resource might still be in use by something,
		// so the cache can't actually count the memory freed until the
		// ResourceHandle pointing to it is destroyed.

		p_Gonner.reset();
		m_HandleLock.unlock();
	}

	void ResourceManager::memoryHasBeenFreed(UINT p_Size, std::string p_ZipPathName)
	{
		m_HandleLock.lock();

		std::pair<UINT, std::string> resource;
		for (auto& res : m_LoadedResources)
		{
			if (res.second == p_ZipPathName)
			{
				resource = res;
				break;
			}
		}

		m_LoadedResources.erase(resource.first);

		m_Allocated -= p_Size;
		m_HandleLock.unlock();
	}

	std::vector<std::string> ResourceManager::match(const std::string p_Pattern, std::string p_GUID)
	{
		std::vector<std::string> matchingNames;
		if (m_FileMap.size() <= 0)
			return matchingNames;

		IResourceFile *resourceFile = nullptr;
		for each (auto &res in m_FileMap)
		{
			if (res.first == p_GUID)
				resourceFile = res.second;
		}

		if (!resourceFile)
			return matchingNames;

		int numFiles = resourceFile->getNumResources();
		for (int i = 0; i < numFiles; ++i)
		{
			std::string name = resourceFile->getResourceName(i);
			std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
			if (wildcardMatch(p_Pattern.c_str(), name.c_str()))
			{
				matchingNames.push_back(name);
			}
		}
		return matchingNames;
	}

	bool ResourceManager::wildcardMatch(const char *pat, const char *str) {
		int i, star;

	new_segment:

		star = 0;
		if (*pat == '*') {
			star = 1;
			do { pat++; } while (*pat == '*'); /* enddo */
		} /* endif */

	test_match:

		for (i = 0; pat[i] && (pat[i] != '*'); i++) {
			//if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
			if (str[i] != pat[i]) {
				if (!str[i]) return 0;
				if ((pat[i] == '?') && (str[i] != '.')) continue;
				if (!star) return 0;
				str++;
				goto test_match;
			}
		}
		if (pat[i] == '*') {
			str += i;
			pat += i;
			goto new_segment;
		}
		if (!str[i]) return 1;
		if (i && pat[i - 1] == '*') return 1;
		if (!star) return 0;
		str++;
		goto test_match;
	}
}