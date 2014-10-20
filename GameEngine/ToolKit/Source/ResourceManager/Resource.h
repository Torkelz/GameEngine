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
		std::string m_ZipName;

		Resource();
		Resource(const std::string &p_Name, const std::string &p_ZipName);
	};
}