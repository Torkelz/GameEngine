#pragma once
#include <list>
#include <memory>
#include <map>
#include <vector>

class IResourceFile;
class IResourceLoader;
class ResHandle;
class Resource;


//
//  class ResCache										- Chapter 8, page 225
//
typedef std::list< std::shared_ptr <ResHandle > > ResHandleList;					// lru list
typedef std::map<std::string, std::shared_ptr < ResHandle  > > ResHandleMap;		// maps indentifiers to resource data
typedef std::list< std::shared_ptr < IResourceLoader > > ResourceLoaders;

class ResourceManager
{
	friend class ResHandle;

	ResHandleList m_Lru;							// lru list
	ResHandleMap m_Resources;
	ResourceLoaders m_ResourceLoaders;

	IResourceFile *m_File;

	unsigned int			m_CacheSize;			// total memory size
	unsigned int			m_Allocated;			// total memory allocated

protected:

	bool makeRoom(unsigned int p_Size);
	char *allocate(unsigned int p_Size);
	void free(std::shared_ptr<ResHandle> p_Gonner);

	std::shared_ptr<ResHandle> load(Resource * p_R);
	std::shared_ptr<ResHandle> find(Resource * p_R);
	void update(std::shared_ptr<ResHandle> p_Handle);

	void freeOneResource();
	void memoryHasBeenFreed(unsigned int p_Size);
	bool wildcardMatch(const char *pat, const char *str);

public:
	ResourceManager(const unsigned int p_SizeInMb, IResourceFile *p_File);
	virtual ~ResourceManager();

	bool init();

	void registerLoader(std::shared_ptr<IResourceLoader> p_Loader);

	std::shared_ptr<ResHandle> getHandle(Resource * p_R);

	//int preload(const std::string p_Pattern, void(*progressCallback)(int, bool &));
	std::vector<std::string> match(const std::string p_Pattern);

	void flush(void);

	//bool isUsingDevelopmentDirectories(void) const { GCC_ASSERT(m_File); return m_File->isUsingDevelopmentDirectories(); }

};
