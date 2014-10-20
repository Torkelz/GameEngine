#pragma once
#include <list>
#include <memory>
#include <map>
#include <vector>

#include "Spinlock.h"

namespace Res
{
	class IResourceFile;
	class IResourceLoader;
	class ResourceHandle;
	class Resource;

	//
	//  class ResCache										- Chapter 8, page 225
	//
	typedef std::list<std::shared_ptr <ResourceHandle>> ResHandleList;					// lru list
	typedef std::map<std::string, std::shared_ptr<ResourceHandle>> ResHandleMap;		// maps indentifiers to resource data
	typedef std::list<std::shared_ptr<IResourceLoader>> ResourceLoaders;

	class ResourceManager
	{
		friend class ResourceHandle;
	
	public:
		typedef unsigned int UINT;
	
	private:
		ResHandleList m_Lru;							// lru list
		ResHandleMap m_Resources;
		ResourceLoaders m_ResourceLoaders;
		std::map<std::string, IResourceFile*> m_FileMap;
		std::map<std::string, UINT> m_GUID_Table;

		UINT m_CacheSize;			// total memory size
		UINT m_Allocated;			// total memory allocated
		SpinLock m_AllocatedLock;	// Thread safing the m_Allocated variable
		SpinLock m_MakeRoomLock;	// Thread safing the makeRoom function.
		SpinLock m_ZipLibLock;		// Thread safing zib library load of header.

	protected:
		bool makeRoom(UINT p_Size);
		char *Allocate(UINT p_Size);

		std::shared_ptr<ResourceHandle> load(Resource *p_R);
		std::shared_ptr<ResourceHandle> find(Resource *p_R);
		void update(std::shared_ptr<ResourceHandle> p_Handle);

		void freeOneResource(void);
		void memoryHasBeenFreed(UINT p_Size);
		bool wildcardMatch(const char *p_Pattern, const char *p_String);

	public:
		ResourceManager(UINT p_Cache);
		virtual ~ResourceManager(void);

		/*
		* Only for testing purposes.
		* @return the total memory allocated by resources
		*/
		UINT getAllocated(void);

		void init(void);
		bool loadZipLib(IResourceFile *p_Resource, std::string p_ZipLib);

		void registerLoader(std::shared_ptr<IResourceLoader> p_Loader);

		std::shared_ptr<ResourceHandle> getHandle(Resource *p_R);

		//int preload(const std::string p_Pattern, void(*progressCallback)(int, bool &));
		std::vector<std::string> match(const std::string p_Pattern, std::string p_GUID);

		void flush(void);

		void Free(std::shared_ptr<ResourceHandle> p_Gonner);

		//bool isUsingDevelopmentDirectories(void) const { GCC_ASSERT(m_File); return m_File->isUsingDevelopmentDirectories(); }
	};
}