#pragma once
#include <list>
#include <memory>
#include <map>
#include <vector>

namespace Res
{
	class IResourceFile;
	class IResourceLoader;
	class ResourceHandle;
	class Resource;

	//
	//  class ResCache										- Chapter 8, page 225
	//
	typedef std::list< std::shared_ptr <ResourceHandle>> ResHandleList;					// lru list
	typedef std::map<std::string, std::shared_ptr<ResourceHandle>> ResHandleMap;		// maps indentifiers to resource data
	typedef std::list< std::shared_ptr<IResourceLoader>> ResourceLoaders;

	class ResourceManager
	{
		friend class ResourceHandle;
	
	public:
		typedef unsigned int UINT;
	
	private:
		ResHandleList m_Lru;							// lru list
		ResHandleMap m_Resources;
		ResourceLoaders m_ResourceLoaders;
		IResourceFile *m_File;

		UINT m_CacheSize;			// total memory size
		UINT m_Allocated;			// total memory allocated

	protected:

		bool makeRoom(UINT p_Size);
		char *Allocate(UINT p_Size);
		void Free(std::shared_ptr<ResourceHandle> p_Gonner);

		std::shared_ptr<ResourceHandle> load(Resource *p_R);
		std::shared_ptr<ResourceHandle> find(Resource *p_R);
		void update(std::shared_ptr<ResourceHandle> p_Handle);

		void freeOneResource(void);
		void memoryHasBeenFreed(UINT p_Size);
		bool wildcardMatch(const char *p_Pattern, const char *p_String);

	public:
		ResourceManager(UINT p_SizeInMb, IResourceFile *p_File);
		virtual ~ResourceManager(void);

		bool init(void);

		void registerLoader(std::shared_ptr<IResourceLoader> p_Loader);

		std::shared_ptr<ResourceHandle> getHandle(Resource *p_R);

		//int preload(const std::string p_Pattern, void(*progressCallback)(int, bool &));
		std::vector<std::string> match(const std::string p_Pattern);

		void flush(void);

		//bool isUsingDevelopmentDirectories(void) const { GCC_ASSERT(m_File); return m_File->isUsingDevelopmentDirectories(); }

	};
}