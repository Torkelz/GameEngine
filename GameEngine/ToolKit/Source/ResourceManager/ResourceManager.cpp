#include "ResourceManager.h"
#include "IResourceFile.h"
#include "IResourceLoader.h"
#include "DefaultResourceLoader.h"
#include "Resource.h"
#include "ResourceHandle.h"
#include "Macros.h"


#include <algorithm>
#include <cctype>


namespace Res
{
	//
	// ResourceManager::ResourceManager							- Chapter 8, page 227
	//
	ResourceManager::ResourceManager(UINT p_SizeInMb) :
		m_CacheSize(p_SizeInMb * 1024 * 1024), m_Allocated(0)
	{		
	}

	//
	// ResourceManager::~ResourceManager							- Chapter 8, page 227
	//
	ResourceManager::~ResourceManager(void)
	{
		while (!m_Lru.empty())
		{
			freeOneResource();
		}

		for (auto &res : m_FileMap)
		{
			SAFE_DELETE(res.second);
		}
	}

	//
	// ResourceManager::Init								- Chapter 8, page 227
	//
	void ResourceManager::init(void)
	{
		registerLoader(std::shared_ptr<IResourceLoader>(new DefaultResourceLoader()));
	}

	bool ResourceManager::loadResource(IResourceFile *p_Resource, std::string p_GUID)
	{
		if (p_Resource->open())
		{
			m_FileMap.insert(std::pair<std::string, IResourceFile*>(p_GUID, p_Resource));

			return true;
		}

		return false;
	}

	//
	// ResourceManager::RegisterLoader						- Chapter 8, page 225
	// 
	//    The loaders are discussed on the page refereced above - this method simply adds the loader
	//    to the resource cache.
	//
	void ResourceManager::registerLoader(std::shared_ptr<IResourceLoader> p_Loader)
	{
		m_ResourceLoaders.push_front(p_Loader);
	}


	//
	// ResourceManager::GetHandle							- Chapter 8, page 227
	//
	std::shared_ptr<ResourceHandle> ResourceManager::getHandle(Resource *p_R, std::string p_GUID)
	{
		std::shared_ptr<ResourceHandle> handle(find(p_R));
		if (handle == nullptr)
		{
			handle = load(p_R, p_GUID);
			//GCC_ASSERT(handle);
		}
		else
		{
			update(handle);
		}
		return handle;
	}

	//
	// ResourceManager::Load								- Chapter 8, page 228-230
	//
	std::shared_ptr<ResourceHandle> ResourceManager::load(Resource *p_R, std::string p_GUID)
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
			//GCC_ASSERT(loader && _T("Default resource loader not found!"));
			return handle;		// Resource not loaded!
		}

		IResourceFile *file = nullptr;
		for each (auto &res in m_FileMap)
		{
			if (res.first == p_GUID)
			{
				file = res.second;
			}
		}
		if (!file)
			return std::shared_ptr<ResourceHandle>();

		int rawSize = file->getRawResourceSize(*p_R);
		if (rawSize < 0)
		{
			//GCC_ASSERT(rawSize > 0 && "Resource size returned -1 - Resource not found");
			return std::shared_ptr<ResourceHandle>();
		}

		int allocSize = rawSize + ((loader->addNullZero()) ? (1) : (0));
		char *rawBuffer = loader->useRawFile() ? Allocate(allocSize) : new char[allocSize];
		memset(rawBuffer, 0, allocSize);

		if (rawBuffer == NULL || file->getRawResource(*p_R, rawBuffer) == 0)
		{
			// resource cache out of memory
			return std::shared_ptr<ResourceHandle>();
		}

		char *buffer = NULL;
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
			if (rawBuffer == NULL || buffer == NULL)
			{
				// resource cache out of memory
				return std::shared_ptr<ResourceHandle>();
			}
			handle = std::shared_ptr<ResourceHandle>(new ResourceHandle(*p_R, buffer, size, this));
			bool success = loader->loadResource(rawBuffer, rawSize, handle);

			// [mrmike] - This was added after the chapter went to copy edit. It is used for those
			//            resoruces that are converted to a useable format upon load, such as a compressed
			//            file. If the raw buffer from the resource file isn't needed, it shouldn't take up
			//            any additional memory, so we release it.
			//
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
		}

		//GCC_ASSERT(loader && _T("Default resource loader not found!"));
		return handle;		// ResourceManager is out of memory!
	}

	//
	// ResourceManager::Find									- Chapter 8, page 228
	//
	std::shared_ptr<ResourceHandle> ResourceManager::find(Resource *p_R)
	{
		ResHandleMap::iterator i = m_Resources.find(p_R->m_Name);
		if (i == m_Resources.end())
			return std::shared_ptr<ResourceHandle>();

		return i->second;
	}

	//
	// ResourceManager::Update									- Chapter 8, page 228
	//
	void ResourceManager::update(std::shared_ptr<ResourceHandle> p_Handle)
	{
		m_Lru.remove(p_Handle);
		m_Lru.push_front(p_Handle);
	}



	//
	// ResourceManager::Allocate								- Chapter 8, page 230
	//
	char *ResourceManager::Allocate(UINT p_Size)
	{
		if (!makeRoom(p_Size))
			return NULL;

		char *mem = new char[p_Size];
		if (mem)
		{
			m_Allocated += p_Size;
		}

		return mem;
	}


	//
	// ResourceManager::FreeOneResource						- Chapter 8, page 231
	//
	void ResourceManager::freeOneResource(void)
	{
		ResHandleList::iterator gonner = m_Lru.end();
		gonner--;

		std::shared_ptr<ResourceHandle> handle = *gonner;

		m_Lru.pop_back();
		m_Resources.erase(handle->m_Resource.m_Name);
		// Note - you can't change the resource cache size yet - the resource bits could still actually be
		// used by some sybsystem holding onto the ResourceHandle. Only when it goes out of scope can the memory
		// be actually free again.
	}



	//
	// ResourceManager::Flush									- not described in the book
	//
	//    Frees every handle in the cache - this would be good to call if you are loading a new
	//    level, or if you wanted to force a refresh of all the data in the cache - which might be 
	//    good in a development environment.
	//
	void ResourceManager::flush(void)
	{
		while (!m_Lru.empty())
		{
			std::shared_ptr<ResourceHandle> handle = *(m_Lru.begin());
			Free(handle);
			m_Lru.pop_front();
		}
	}


	//
	// ResourceManager::MakeRoom									- Chapter 8, page 231
	//
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
				return false;

			freeOneResource();
		}

		return true;
	}

	//
	//	ResourceManager::Free									- Chapter 8, page 228
	//
	void ResourceManager::Free(std::shared_ptr<ResourceHandle> p_Gonner)
	{
		m_Lru.remove(p_Gonner);
		m_Resources.erase(p_Gonner->m_Resource.m_Name);
		// Note - the resource might still be in use by something,
		// so the cache can't actually count the memory freed until the
		// ResourceHandle pointing to it is destroyed.

		//m_Allocated -= gonner->m_Resource.m_size;
		//delete gonner;
	}

	//
	//  ResourceManager::MemoryHasBeenFreed					- not described in the book
	//
	//     This is called whenever the memory associated with a resource is actually freed
	//
	void ResourceManager::memoryHasBeenFreed(UINT p_Size)
	{
		m_Allocated -= p_Size;
	}

	//
	// ResourceManager::Match									- not described in the book
	//
	//   Searches the resource cache assets for files matching the pattern. Useful for providing a 
	//   a list of levels for a main menu screen, for example.
	//
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


	//
	// ResourceManager::Preload								- Chapter 8, page 236
	//
	//int ResourceManager::preload(const std::string p_Pattern, void(*progressCallback)(int, bool &))
	//{
	//	if (m_File == NULL)
	//		return 0;
	//
	//	int numFiles = m_File->getNumResources();
	//	int loaded = 0;
	//	bool cancel = false;
	//	for (int i = 0; i<numFiles; ++i)
	//	{
	//		Resource resource(m_File->getResourceName(i));
	//
	//		if (wildcardMatch(p_Pattern.c_str(), resource.m_Name.c_str()))
	//		{
	//			std::shared_ptr<ResourceHandle> handle = g_pApp->m_ResourceManager->getHandle(&resource);
	//			++loaded;
	//		}
	//
	//		if (progressCallback != NULL)
	//		{
	//			progressCallback(i * 100 / numFiles, cancel);
	//		}
	//	}
	//	return loaded;
	//}



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