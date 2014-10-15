#pragma once
#include "WrapperFactory.h"
#include "XMFloatUtil.h"
#include "Render.h"
#include "ModifyMesh.h"


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

public:
	Level();
	~Level();
	void initialize(Render *p_Render);
	void update();
	void draw();

	
private:
	Vertex* createBox(int size, DirectX::XMVECTOR center);

	inline Vertex createVertex(DirectX::XMVECTOR _position, DirectX::XMVECTOR _normal)
	{
		Vertex v;

		DirectX::XMStoreFloat3(&v.position, _position);
		DirectX::XMStoreFloat3(&v.normal, _normal);

		return v;
	}
};

