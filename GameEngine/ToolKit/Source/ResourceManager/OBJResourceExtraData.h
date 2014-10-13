#pragma once
#include "IResourceExtraData.h"
#include "Resource.h"

namespace Res
{
	class OBJResourceExtraData : public IResourceExtraData
	{
		friend class OBJResourceLoader;

	public:
		OBJResourceExtraData() {}
		virtual ~OBJResourceExtraData() {}

		virtual std::string toString() { return "OBJResourceExtraData"; }

		void setMTLFile(Resource res)
		{ 
			m_MTLFile = res;

		}

	protected:

	private:
		Resource m_MTLFile;

	};
}