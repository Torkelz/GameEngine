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

	int lamp;

	Res::ResourceManager *m_ResourceManager;


	Allocator::LinearAllocator *m_LinAlloc;
	Particles m_Particles;
public:
	Level();
	~Level();
	void initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager);
	void update(float p_Dt);
	void draw();
		
private:
};

