#pragma once
#include <list>
#include <memory>
#include <map>
#include <vector>

namespace Res
{
	class IResourceFile;
	class IResourceLoader;
	class ResHandle;
	class Resource;

	//
	//  class ResCache										- Chapter 8, page 225
	//
	typedef std::list< std::shared_ptr <ResHandle>> ResHandleList;					// lru list
	typedef std::map<std::string, std::shared_ptr<ResHandle>> ResHandleMap;		// maps indentifiers to resource data
	typedef std::list< std::shared_ptr<IResourceLoader>> ResourceLoaders;

	class ResourceManager
	{
		friend class ResHandle;
	
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
		char *allocate(UINT p_Size);
		void free(std::shared_ptr<ResHandle> p_Gonner);

		std::shared_ptr<ResHandle> load(Resource *p_R);
		std::shared_ptr<ResHandle> find(Resource *p_R);
		void update(std::shared_ptr<ResHandle> p_Handle);

		void freeOneResource(void);
		void memoryHasBeenFreed(UINT p_Size);
		bool wildcardMatch(const char *p_Pattern, const char *p_String);

	public:
		ResourceManager(const UINT p_SizeInMb, IResourceFile *p_File);
		virtual ~ResourceManager(void);

		bool init(void);

		void registerLoader(std::shared_ptr<IResourceLoader> p_Loader);

		std::shared_ptr<ResHandle> getHandle(Resource *p_R);

		//int preload(const std::string p_Pattern, void(*progressCallback)(int, bool &));
		std::vector<std::string> match(const std::string p_Pattern);

		void flush(void);

		//bool isUsingDevelopmentDirectories(void) const { GCC_ASSERT(m_File); return m_File->isUsingDevelopmentDirectories(); }

	};
}