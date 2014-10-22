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
	Vector3 *m_CameraPos;
	int lamp;
	bool green;
	bool red;
	bool blue;
	Res::ResourceManager *m_ResourceManager;
	Res::ResourceZipFile m_zip;
	Res::Resource m_OptimusRed;
	Res::Resource m_OptimusGreen;
	Res::Resource m_OptimusBlue;
	Res::Resource m_OptimusObj;
public:
	Level();
	~Level();
	void initialize(Render *p_Render, Res::ResourceManager *p_ResourceManager, Vector3 *m_CameraPos);
	void update();
	void draw();
		
private:
};

