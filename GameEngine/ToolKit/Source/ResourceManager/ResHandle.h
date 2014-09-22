#pragma once

#include <string>
#include <memory>

#include "Resource.h"

namespace Res
{
	class IResourceExtraData;

	//
	//  class ResHandle			- Chapter 8, page 222
	//
	class ResHandle
	{
		friend class ResourceManager;
	public:
		typedef unsigned int UINT;

	protected:
		Resource m_Resource;
		char *m_Buffer;
		UINT m_Size;
		std::shared_ptr<IResourceExtraData> m_Extra;
		ResourceManager *m_ResCache;

	public:
		ResHandle(Resource &p_Resource, char *p_Buffer, UINT p_Size, ResourceManager *p_ResCache);

		virtual ~ResHandle(void);

		const std::string getName(void);
		UINT size(void) const;
		char *buffer(void) const;
		char *writableBuffer(void);

		std::shared_ptr<IResourceExtraData> getExtra(void);
		void setExtra(std::shared_ptr<IResourceExtraData> extra);
	};
}