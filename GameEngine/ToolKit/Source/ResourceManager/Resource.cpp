#include "Resource.h"

#include <cctype>
#include <algorithm>

namespace Res
{
	Resource::Resource(){}

	Resource::Resource(const std::string &p_Name, const std::string &p_ZipName) :
		m_Name(p_Name), m_ZipName(p_ZipName)
	{
		std::transform(m_Name.begin(), m_Name.end(), m_Name.begin(), (int(*)(int)) std::tolower);
	}
}