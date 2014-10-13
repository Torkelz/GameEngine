#pragma once
#include "IResourceExtraData.h"
namespace Res
{
	class OBJResourceExtraData : public IResourceExtraData
	{
		friend class OBJResourceLoader;

	public:
		OBJResourceExtraData();
		virtual ~OBJResourceExtraData() {}

		virtual std::string toString() { return "OBJResourceExtraData"; }

	protected:

	};
}