#include "OBJResourceLoader.h"
#include "ResourceHandle.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>
#include <DirectXMath.h>

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
		return p_RawSize * 0.5f; //Might be wrong!
	}

	bool OBJResourceLoader::loadResource(char *p_RawBuffer, UINT p_RawSize, std::shared_ptr<ResourceHandle> p_Handle)
	{
		return parseOBJ(p_RawBuffer, p_RawSize, p_Handle);
	}

	bool OBJResourceLoader::parseOBJ(char *p_ObjStream, size_t p_Length, std::shared_ptr<ResourceHandle> p_Handle)
	{
		using namespace DirectX;

		std::shared_ptr<OBJResourceExtraData> extra(new OBJResourceExtraData());
		p_Handle->setExtra(extra);

		std::vector<XMFLOAT3> positions;
		std::vector<XMFLOAT3> normals;
		std::vector<XMFLOAT2> texCoords;

		std::string line;
		std::string prefix;
		std::map<int, int> vertexMap;


		std::map<int, std::string> faceGroups;
		std::vector<int> index;
		std::vector<Vertex> vertices;

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
				std::string c = p_Handle->getContainerName();

				extra->setMTLFile(Resource(materialPath, c));
			}
			else if (prefix == "usemtl")
			{
				std::string s;
				lineStream >> s;
				faceGroups.insert(std::make_pair(index.size(), s));
			}
			else if (prefix == "v")
			{
				XMFLOAT3 pos;
				lineStream >> pos.x >> pos.y >> pos.z;
				positions.push_back(pos);
			}
			else if (prefix == "vn")
			{
				XMFLOAT3 normal;
				lineStream >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (prefix == "vt")
			{
				XMFLOAT2 texC;
				lineStream >> texC.x >> texC.y;
				texC.y *= -1;
				texCoords.push_back(texC);
			}
			else if (prefix == "f")
			{
				Vertex vertex;
				char slash;
				bool calcNormal = false;
				for (UINT iFace = 0; iFace < 3; iFace++)
				{
					int iValues[3] = { -1, -1, -1 };

					for (unsigned int i = 0; i < 3; i++)
					{

						if (lineStream.peek() == '/')
						{
							lineStream >> slash;
							continue;
						}

						lineStream >> iValues[i];
						if (lineStream.peek() == '/')
						{
							lineStream >> slash;
						}
					}

					int hash = 73 * iValues[0];
					if (iValues[1] >= 0) hash += 89 * iValues[1];
					if (iValues[2] >= 0) hash += 151 * iValues[2];


					if (vertexMap.count(hash) > 0)
					{
						index.push_back(vertexMap[hash]);
					}
					else
					{
						vertex.pos = positions[iValues[0] - 1];
						if (iValues[2] > 0)
						{
							vertex.normal = normals[iValues[2] - 1];
						}
						else
							calcNormal = true;

						vertex.texCoords = (iValues[1] > 0) ? texCoords[iValues[1] - 1] : XMFLOAT2(-1, -1);
						vertices.push_back(vertex);
						index.push_back(vertices.size() - 1);

						vertexMap.insert(std::make_pair(hash, vertices.size() - 1));
					}					
				}

				if (calcNormal)
				{
					XMFLOAT3 normal;
					XMVECTOR v1, v2;
					v1 = XMLoadFloat3(&vertices.at(vertices.size() - 3).pos) - XMLoadFloat3(&vertices.at(vertices.size() - 2).pos);
					v2 = XMLoadFloat3(&vertices.at(vertices.size() - 3).pos) - XMLoadFloat3(&vertices.at(vertices.size() - 1).pos);

					XMStoreFloat3(&normal, XMVector3Normalize(XMVector3Cross(v1, v2)));

					vertices.at(vertices.size() - 1).normal = normal;
					vertices.at(vertices.size() - 2).normal = normal;
					vertices.at(vertices.size() - 3).normal = normal;
				}
			}
		}
		extra->setFaceGroupData(faceGroups);
		extra->setBufferSeperator(index.size() * sizeof(int));
		size_t totalSize = index.size() * sizeof(int) + vertices.size()*sizeof(Vertex);
		extra->setBufferTotalSize(totalSize);

		memcpy(p_Handle->writableBuffer(), index.data(), index.size()*sizeof(int));
		memcpy(p_Handle->writableBuffer() + index.size() * sizeof(int), vertices.data(), vertices.size()*sizeof(Vertex));

		//std::vector<int> l; 
		//l.resize(index.size());
		//std::copy(p_Handle->buffer(), p_Handle->buffer() + index.size(), l.begin());
		//l.assign(p_Handle->buffer(), p_Handle->buffer() + index.size());

		//int ret = memcmp(p_Handle->writableBuffer(), index.data(), index.size() * sizeof(int));

		
 		return true;
	}
}
