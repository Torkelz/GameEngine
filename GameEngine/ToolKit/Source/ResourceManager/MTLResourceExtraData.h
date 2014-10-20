#pragma once
#include "IResourceExtraData.h"
#include "Resource.h"
#include "Material.h"
#include <vector>

class MTLResourceLoader;

namespace Res
{
	class MTLResourceExtraData : public IResourceExtraData
	{

	public:
		MTLResourceExtraData() {}
		virtual ~MTLResourceExtraData() {}

		virtual std::string toString() { return "MTLResourceExtraData"; }

		void setMaterials(std::vector<Material> p_Material)
		{
			m_Materials = p_Material;
		}

		std::vector<Material> getMaterials()
		{
			return m_Materials;
		}

	protected:

	private:
		std::vector<Material> m_Materials;
	};
}