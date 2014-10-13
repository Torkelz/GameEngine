#include "OBJResourceLoader.h"
#include "ResourceHandle.h"
#include <vector>
#include <sstream>
#include <fstream>

namespace Res
{
	std::string OBJResourceLoader::getPattern(void)
	{
		return "*.obj";
	}

	bool OBJResourceLoader::useRawFile(void)
	{
		return false;
	}

	bool OBJResourceLoader::discardRawBufferAfterLoad(void)
	{
		return true;
	}

	OBJResourceLoader::UINT OBJResourceLoader::getLoadedResourceSize(char *p_RawBuffer, UINT p_RawSize)
	{
		return p_RawSize; //Might be wrong!
	}

	bool OBJResourceLoader::loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle)
	{
		return parseOBJ(p_RawBuffer, p_RawSize, p_Handle);
	}

	bool OBJResourceLoader::parseOBJ(char *p_ObjStream, size_t p_Length, std::shared_ptr<ResourceHandle> p_Handle)
	{
		std::shared_ptr<OBJResourceExtraData> extra = std::static_pointer_cast<OBJResourceExtraData>(p_Handle->getExtra());

		std::vector<Vec3> positions;
		std::vector<Vec3> normals;
		std::vector<Vec2> texCoords;
		std::vector<Vertex> vertices;

		std::string srcMaterial;
		std::string line;
		std::string prefix;

		std::stringstream fileStream;
		fileStream << p_ObjStream;
		while (std::getline(fileStream, line))
		{
			prefix = "NULL";
			std::stringstream lineStream;
			lineStream << line;
			lineStream >> prefix;
			if (prefix == "NULL" || prefix == "#")
				continue;
			if (prefix == "mtllib")
			{
				std::string s;
				lineStream >> s;
				std::string filepath = p_Handle->getName();
				std::string materialPath = filepath.substr(0, filepath.find_last_of("\\/")) + "\\" + s;
				int dummy = 0 ;
			}
			else if (prefix == "v")
			{
				Vec3 pos;
				lineStream >> pos.x >> pos.y >> pos.z;
				positions.push_back(pos);
			}
			else if (prefix == "vn")
			{
				Vec3 normal;
				lineStream >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (prefix == "vt")
			{
				Vec2 texC;
				lineStream >> texC.x >> texC.y;
				texC.y *= -1;
				texCoords.push_back(texC);
			}
			else if (prefix == "f")
			{
				UINT iPosition, iTexCoord, iNormal;
				Vertex vertex;
				char slash;
				for (UINT iFace = 0; iFace < 3; iFace++)
				{
					lineStream >> iPosition >> slash >> iTexCoord >> slash >> iNormal;
					vertex.pos = positions[iPosition - 1];
					vertex.normal = normals[iNormal - 1];
					vertex.texCoords = texCoords[iTexCoord - 1];
					vertices.push_back(vertex);
				}
			}
		}
		positions.clear();
		normals.clear();
		texCoords.clear();

		return true;
	}
}
