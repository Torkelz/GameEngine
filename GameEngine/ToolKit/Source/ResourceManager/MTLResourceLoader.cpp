#include "MTLResourceLoader.h"
#include "ResourceHandle.h"
#include "Material.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>

namespace Res
{
	std::string MTLResourceLoader::getPattern(void)
	{
		return "*.mtl";
	}

	bool MTLResourceLoader::useRawFile(void)
	{
		return false;
	}

	bool MTLResourceLoader::discardRawBufferAfterLoad(void)
	{
		return true;
	}

	MTLResourceLoader::UINT MTLResourceLoader::getLoadedResourceSize(char *p_RawBuffer, UINT p_RawSize)
	{
		return p_RawSize; //Might be wrong!
	}

	bool MTLResourceLoader::loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle)
	{
		return parseMTL(p_RawBuffer, p_RawSize, p_Handle);
	}

	bool MTLResourceLoader::parseMTL(char *p_MTLStream, size_t p_Length, std::shared_ptr<ResourceHandle> p_Handle)
	{
		using namespace DirectX;

		std::shared_ptr<MTLResourceExtraData> extra(new MTLResourceExtraData());
		p_Handle->setExtra(extra);

		std::vector<Material> mats;
		std::string line;
		std::string prefix;

		membuf mem(p_MTLStream, p_MTLStream + p_Length);
		std::istream fileStream(&mem);

		while (std::getline(fileStream, line))
		{
			prefix = "NULL";
			std::stringstream lineStream;
			lineStream << line;
			lineStream >> prefix;
			if (prefix == "NULL" || prefix == "#")
				continue;
			if (prefix == "newmtl")
			{
				Material m;
				mats.push_back(m);
				lineStream >> mats.back().name;
			}
			else if (prefix == "Kd")
			{
				lineStream >> mats.back().Kd.x >> mats.back().Kd.y >> mats.back().Kd.z;

			}
			else if (prefix == "Ka")
			{
				lineStream >> mats.back().Ka.x >> mats.back().Ka.y >> mats.back().Ka.z;
			}
			else if (prefix == "Ks")
			{
				lineStream >> mats.back().Ks.x >> mats.back().Ks.y >> mats.back().Ks.z;
			}
			else if (prefix == "Ns")
			{
				lineStream >> mats.back().Ns;
			}
			else if (prefix == "d")
			{
				lineStream >> mats.back().d;
			}
			else if (prefix == "illum")
			{
				lineStream >> mats.back().illum;
			}

			else if (prefix == "map_Kd")
			{
				std::string s;
				lineStream >> s;

				std::string filepath = p_Handle->getName();
				size_t lastPath = filepath.find_last_of("\\/");

				std::string materialPath = filepath.substr(0, lastPath) + "\\" + s;
				if (lastPath == std::string::npos)
					materialPath = s;

				mats.back().map_Kd = Resource(materialPath, p_Handle->getContainerName());
			}
		}

		if (mats.empty())
		{
			throw std::length_error("Error while parsing " + p_Handle->getName() + ", probably cause is carrier returns in the file. Replace all \r\n with \n.");
		}

		extra->setMaterials(mats);

		memcpy(p_Handle->writableBuffer(), mats.data(), mats.size()*sizeof(Material));
		
		int ret = memcmp(p_Handle->writableBuffer(), mats.data(), mats.size() * sizeof(Material));

		return (ret == 0) ? true : false;
	}
}
