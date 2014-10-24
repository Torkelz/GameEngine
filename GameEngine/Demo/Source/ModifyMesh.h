#pragma once
#include "Render.h"

class ModifyMesh
{
	friend class Render;
private:
	Render *m_Render;
	static std::unique_ptr<ModifyMesh> m_Instance;
public:
	/**
	* Initialized the ModifyMesh singleton.
	* @param p_Render, valid pointer to Render.
	*/
	static void initialize(Render *p_Render);

	/**
	* Sets a mesh instance's position.
	* @param p_InstanceId, the mesh instance's graphical identifier.
	* @param p_Position, Vector3 in world coordinates.
	*/
	static void setMeshPosition(Render::UINT p_InstanceId, Vector3 p_Position);

	/**
	* Sets a mesh instance's rotation.
	* @param p_InstanceId, the mesh instance's graphical identifier.
	* @param p_Rotation, Vector3(pitch,yaw,roll) in degrees.
	*/
	static void setMeshRotation(Render::UINT p_InstanceId, Vector3 p_YawPitchRoll);

	/**
	* Sets a mesh instance's scale.
	* @param p_InstanceId, the mesh instance's graphical identifier.
	* @param p_Scale, Vector(ScaleX,ScaleY,ScaleZ).
	*/
	static void setMeshScale(Render::UINT p_InstanceId, Vector3 p_Scale);

	/**
	* Gets the singleton instance of ModifyMesh.
	* @return, returns a ModifyMesh pointer.
	*/
	static ModifyMesh *getInstance(void);
};