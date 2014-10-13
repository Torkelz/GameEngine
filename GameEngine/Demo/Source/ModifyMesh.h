#pragma once
#include "Render.h"

class ModifyMesh
{
	friend class Render;
private:
	Render *m_Render;
	static std::unique_ptr<ModifyMesh> m_Instance;
public:
	static void initialize(Render *p_Render);

	static void setMeshPosition(Render::UINT p_InstanceId, Vector3 p_Position);
	static void setMeshRotation(Render::UINT p_InstanceId, Vector3 p_YawPitchRoll);
	static void setMeshScale(Render::UINT p_InstanceId, Vector3 p_Scale);
	static ModifyMesh *getInstance();
};