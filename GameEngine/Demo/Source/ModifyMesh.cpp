#include "ModifyMesh.h"
#include "Render.h"

std::unique_ptr<ModifyMesh> ModifyMesh::m_Instance;

void ModifyMesh::setMeshPosition(UINT p_InstanceId, Vector3 p_Position)
{
	getInstance()->m_Render->getMeshInstance(p_InstanceId)->setPosition(p_Position);
}

void ModifyMesh::setMeshRotation(UINT p_InstanceId, Vector3 p_YawPitchRoll)
{
	getInstance()->m_Render->getMeshInstance(p_InstanceId)->setRotation(p_YawPitchRoll);
}

void ModifyMesh::setMeshScale(UINT p_InstanceId, Vector3 p_Scale)
{
	getInstance()->m_Render->getMeshInstance(p_InstanceId)->setScale(p_Scale);
}

void ModifyMesh::initialize(Render *p_Render)
{
	if (!m_Instance)
	{
		m_Instance.reset(new ModifyMesh());
	}
	m_Instance.get()->m_Render = p_Render;
}

ModifyMesh *ModifyMesh::getInstance()
{

	if (!m_Instance)
	{
		m_Instance.reset(new ModifyMesh());
	}

	return m_Instance.get();
}