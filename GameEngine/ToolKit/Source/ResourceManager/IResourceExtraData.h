#pragma once

#include <string>

namespace Res
{
	class IResourceExtraData
	{
	public:
		virtual std::string toString(void) = 0;
	};
}