#pragma once

#include <string>

namespace Res
{
	//
	//  class Resource			- Chapter 8, page 220
	//
	class Resource
	{
	public:
		std::string m_Name;
		Resource(const std::string &p_Name);
	};
}