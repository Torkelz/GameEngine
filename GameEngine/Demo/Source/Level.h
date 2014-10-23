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
	int lamp;
	bool green;
	bool red;
	bool blue;
	Res::ResourceManager *m_ResourceManager;
	//Res::ResourceZipFile m_zip;
	Res::ResourceMcapFile m_mcap;

	Res::Resource m_OptimusRed;
	Res::Resource m_OptimusGreen;
	Res::Resource m_OptimusBlue;

	Allocator::LinearAllocator *m_LinAlloc;
	Particles m_Particles;
	Res::Resource m_OptimusObj;
public:
	Level();
	~Level();
	void initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager, Vector3 *m_CameraPos);
	void update(float p_Dt);
	void draw();
		
private:
};

