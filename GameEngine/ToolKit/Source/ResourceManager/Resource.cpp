#include "Resource.h"

#include <cctype>
#include <algorithm>

namespace Res
{
	Resource::Resource(const std::string &p_Name) :
		m_Name(p_Name)
	{
		std::transform(m_Name.begin(), m_Name.end(), m_Name.begin(), (int(*)(int)) std::tolower);
	}
}