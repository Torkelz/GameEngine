#include "Resource.h"

#include <cctype>
#include <algorithm>

namespace Res
{
	Resource::Resource(const std::string &p_Name, const std::string &p_ZipName) :
		m_Name(p_Name)
	{
		std::transform(m_Name.begin(), m_Name.end(), m_Name.begin(), (int(*)(int)) std::tolower);
		m_ZipName = p_ZipName;
	}
}