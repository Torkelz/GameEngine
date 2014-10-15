#pragma once
#include "IResourceExtraData.h"
#include "Resource.h"
#include <map>

namespace Res
{
	class OBJResourceExtraData : public IResourceExtraData
	{
		friend class OBJResourceLoader;

	public:
		OBJResourceExtraData() {}
		virtual ~OBJResourceExtraData() {}

		virtual std::string toString() { return "OBJResourceExtraData"; }

		void setMTLFile(Resource res)
		{ 
			m_MTLFile = res;
		}
		void setFaceGroupData(std::map<int, std::string> p_Data)
		{
			m_FaceGroups = p_Data;
		}
		void setBufferSeperator(unsigned int p_Seperator)
		{
			m_Seperator = p_Seperator;
		}
		void setBufferTotalSize(unsigned int p_Size)
		{
			m_TotalSize = p_Size;
		}

		Resource getMTLFile()
		{
			return m_MTLFile;
		}
		std::map<int, std::string> getFaceGroupData(std::map<int, std::string> p_Data)
		{
			return m_FaceGroups;
		}
		unsigned int getBufferSeperator()
		{
			return m_Seperator;
		}
		unsigned int getBufferTotalSize()
		{
			return m_TotalSize;
		}

	protected:

	private:
		Resource m_MTLFile;
		std::map<int, std::string> m_FaceGroups;
		unsigned int m_Seperator;
		unsigned int m_TotalSize;
	};
}