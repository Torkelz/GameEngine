#include "ResourceManager.h"
#include "IResourceFile.h"
#include "IResourceLoader.h"
#include "DefaultResourceLoader.h"
#include "Resource.h"
#include "ResHandle.h"
#include "Macros.h"


#include <algorithm>
#include <cctype>



//
// ResourceManager::ResourceManager							- Chapter 8, page 227
//
ResourceManager::ResourceManager(const unsigned int p_SizeInMb, IResourceFile *p_ResFile)
{
	m_CacheSize = p_SizeInMb * 1024 * 1024;				// total memory size
	m_Allocated = 0;									// total memory allocated
	m_File = p_ResFile;
}

//
// ResourceManager::~ResourceManager							- Chapter 8, page 227
//
ResourceManager::~ResourceManager()
{
	while (!m_Lru.empty())
	{
		freeOneResource();
	}
	SAFE_DELETE(m_File);
}

//
// ResourceManager::Init								- Chapter 8, page 227
//
bool ResourceManager::init()
{
	bool retValue = false;
	if (m_File->open())
	{
		registerLoader(std::shared_ptr<IResourceLoader>(new DefaultResourceLoader()));
		retValue = true;
	}
	return retValue;
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
std::shared_ptr<ResHandle> ResourceManager::getHandle(Resource * p_R)
{
	std::shared_ptr<ResHandle> handle(find(p_R));
	if (handle == NULL)
	{
		handle = load(p_R);
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
std::shared_ptr<ResHandle> ResourceManager::load(Resource *p_R)
{
	// Create a new resource and add it to the lru list and map

	std::shared_ptr<IResourceLoader> loader;
	std::shared_ptr<ResHandle> handle;

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

	int rawSize = m_File->getRawResourceSize(*p_R);
	if (rawSize < 0)
	{
		//GCC_ASSERT(rawSize > 0 && "Resource size returned -1 - Resource not found");
		return std::shared_ptr<ResHandle>();
	}

	int allocSize = rawSize + ((loader->addNullZero()) ? (1) : (0));
	char *rawBuffer = loader->useRawFile() ? allocate(allocSize) : new char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if (rawBuffer == NULL || m_File->getRawResource(*p_R, rawBuffer) == 0)
	{
		// resource cache out of memory
		return std::shared_ptr<ResHandle>();
	}

	char *buffer = NULL;
	unsigned int size = 0;

	if (loader->useRawFile())
	{
		buffer = rawBuffer;
		handle = std::shared_ptr<ResHandle>(new ResHandle(*p_R, buffer, rawSize, this));
	}
	else
	{
		size = loader->getLoadedResourceSize(rawBuffer, rawSize);
		buffer = allocate(size);
		if (rawBuffer == NULL || buffer == NULL)
		{
			// resource cache out of memory
			return std::shared_ptr<ResHandle>();
		}
		handle = std::shared_ptr<ResHandle>(new ResHandle(*p_R, buffer, size, this));
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
			return std::shared_ptr<ResHandle>();
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
std::shared_ptr<ResHandle> ResourceManager::find(Resource * p_R)
{
	ResHandleMap::iterator i = m_Resources.find(p_R->m_Name);
	if (i == m_Resources.end())
		return std::shared_ptr<ResHandle>();

	return i->second;
}

//
// ResourceManager::Update									- Chapter 8, page 228
//
void ResourceManager::update(std::shared_ptr<ResHandle> p_Handle)
{
	m_Lru.remove(p_Handle);
	m_Lru.push_front(p_Handle);
}



//
// ResourceManager::Allocate								- Chapter 8, page 230
//
char *ResourceManager::allocate(unsigned int p_Size)
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
void ResourceManager::freeOneResource()
{
	ResHandleList::iterator gonner = m_Lru.end();
	gonner--;

	std::shared_ptr<ResHandle> handle = *gonner;

	m_Lru.pop_back();
	m_Resources.erase(handle->m_Resource.m_Name);
	// Note - you can't change the resource cache size yet - the resource bits could still actually be
	// used by some sybsystem holding onto the ResHandle. Only when it goes out of scope can the memory
	// be actually free again.
}



//
// ResourceManager::Flush									- not described in the book
//
//    Frees every handle in the cache - this would be good to call if you are loading a new
//    level, or if you wanted to force a refresh of all the data in the cache - which might be 
//    good in a development environment.
//
void ResourceManager::flush()
{
	while (!m_Lru.empty())
	{
		std::shared_ptr<ResHandle> handle = *(m_Lru.begin());
		free(handle);
		m_Lru.pop_front();
	}
}


//
// ResourceManager::MakeRoom									- Chapter 8, page 231
//
bool ResourceManager::makeRoom(unsigned int p_Size)
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
void ResourceManager::free(std::shared_ptr<ResHandle> p_Gonner)
{
	m_Lru.remove(p_Gonner);
	m_Resources.erase(p_Gonner->m_Resource.m_Name);
	// Note - the resource might still be in use by something,
	// so the cache can't actually count the memory freed until the
	// ResHandle pointing to it is destroyed.

	//m_Allocated -= gonner->m_Resource.m_size;
	//delete gonner;
}

//
//  ResourceManager::MemoryHasBeenFreed					- not described in the book
//
//     This is called whenever the memory associated with a resource is actually freed
//
void ResourceManager::memoryHasBeenFreed(unsigned int p_Size)
{
	m_Allocated -= p_Size;
}

//
// ResourceManager::Match									- not described in the book
//
//   Searches the resource cache assets for files matching the pattern. Useful for providing a 
//   a list of levels for a main menu screen, for example.
//
std::vector<std::string> ResourceManager::match(const std::string p_Pattern)
{
	std::vector<std::string> matchingNames;
	if (m_File == NULL)
		return matchingNames;

	int numFiles = m_File->getNumResources();
	for (int i = 0; i<numFiles; ++i)
	{
		std::string name = m_File->getResourceName(i);
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
//			std::shared_ptr<ResHandle> handle = g_pApp->m_ResourceManager->getHandle(&resource);
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