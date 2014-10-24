#pragma once
#include "WrapperFactory.h"
#include "XMFloatUtil.h"
#include "Render.h"
#include "ModifyMesh.h"
#include "IAllocators.h"
#include "Particles.h"

class Res::ResourceManager;

class Level
{
private:
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
	};

	Render *m_Render;
	Vector3 *m_CameraPos;
	int m_OptimusHandle;
	bool m_Green;
	bool m_Red;
	bool m_Blue;

	Res::ResourceManager *m_ResourceManager;
	Res::ResourceMcapFile m_Mcap;

	Res::Resource m_OptimusRed;
	Res::Resource m_OptimusGreen;
	Res::Resource m_OptimusBlue;

	Allocator::LinearAllocator *m_LinAlloc;
	Particles m_Particles;
	Res::Resource m_OptimusObj;
public:
	Level(void);
	~Level(void);

	/**
	* Initialize all the levels components and resources.
	* @param p_Render, pointer to a already initialized Render.
	* @param p_ResourceManager, pointer to a already initialized ResourceManager.
	* @param p_CameraPos, pointer to a already initialized camera position.
	*/
	void initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager, Vector3 *p_CameraPos);
	
	/**
	* Updates level specific resources.
	* @param p_Dt, the programs delta time in seconds.
	*/
	void update(float p_Dt);
	
	/**
	* Draws level specific resources.
	*/
	void draw(void);
};

