#pragma once
#include "WrapperFactory.h"
#include "XMFloatUtil.h"
#include "Render.h"
#include "ModifyMesh.h"

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

public:
	Level();
	~Level();
	void initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager);
	void update();
	void draw();
		
private:
};

