#include "Particles.h"
#include "Utilities.h"

#include <algorithm>

#include "Render.h"
#include "Graphics.h"

Particles::Particles() :
m_PoolAllocator(nullptr),
m_AccumulatedTime(0.f)
{
}


Particles::~Particles()
{
	SAFE_DELETE(m_PoolAllocator);
}

void Particles::initialize(char *p_Buffer, unsigned int p_MaxParticles, 
	DirectX::XMFLOAT3 p_EmitPosition, float p_MaxLife, float p_TimePerParticle, Render *p_Render)
{
	//m_PoolAllocator = new Allocator::PoolAllocator(sizeof(Particle), 64, p_MaxParticles + 1);
	m_PoolAllocator = new Allocator::PoolAllocator(p_Buffer, sizeof(Particle), p_MaxParticles);

	m_EmitPosition = p_EmitPosition;
	m_ParticleMaxLife = p_MaxLife;
	m_MaxParticles = p_MaxParticles-1;
	m_TimePerParticle = p_TimePerParticle;
	m_Render = p_Render;


	Buffer::Description bDesc = {};
	bDesc.initData = nullptr;
	bDesc.numOfElements = m_MaxParticles;
	bDesc.sizeOfElement = sizeof(renderParticle);
	bDesc.type = Buffer::Type::VERTEX_BUFFER;
	bDesc.usage = Buffer::Usage::CPU_WRITE_DISCARD;

	m_Buffer = WrapperFactory::getInstance()->createBuffer(bDesc);

	m_Shader = (WrapperFactory::getInstance()->createShader(L".\\Source\\Shader\\Particle.hlsl", 
		"VS,PS,GS", "5_0", ShaderType::VERTEX_SHADER | ShaderType::PIXEL_SHADER | ShaderType::GEOMETRY_SHADER));
	
}

void Particles::update(float p_Dt)
{
	m_AccumulatedTime += p_Dt;
	if (m_AccumulatedTime > m_TimePerParticle)
	{
		m_AccumulatedTime = 0.f;

		emitNewParticle();
	}
	int index = 0;
	for (Particle *p : m_Particles)
	{
		p->elapsedTime += p_Dt;
		using DirectX::operator+;
		using DirectX::operator*;

		DirectX::XMVECTOR v1 = cohesion(p, index);
		DirectX::XMVECTOR v2 = alignment(p, index);
		DirectX::XMVECTOR v3 = separation(p, index);
		DirectX::XMVECTOR v4 = goal(p);

		DirectX::XMVECTOR velocity = XMLoadFloat3(&p->velocity) + v1 + v2 + v3 + v4;

		DirectX::XMStoreFloat3(&p->velocity, DirectX::XMVector3Normalize(velocity));

		
		DirectX::XMStoreFloat3(&p->position, DirectX::XMLoadFloat3(&p->position) + (DirectX::XMLoadFloat3(&p->velocity) * p_Dt * 4));
		index++;
	}
	killOldParticles();
}

void Particles::render()
{
	D3D11_MAPPED_SUBRESOURCE resource = {};
	m_Render->getGraphics()->getDeviceContext()->Map(m_Buffer->getBufferPointer(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &resource);
	renderParticle* mappedShaderParticle = (renderParticle*)resource.pData;
	for (Particle *part : m_Particles)
	{
		mappedShaderParticle->position = part->position;
		mappedShaderParticle->color = part->color;

		mappedShaderParticle++;
	}
	m_Render->getGraphics()->getDeviceContext()->Unmap(m_Buffer->getBufferPointer(), 0);


	m_Render->setCameraBuffers();

	m_Buffer->setBuffer(0);
	m_Shader->setShader();

	m_Render->getGraphics()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_Render->getGraphics()->getDeviceContext()->Draw(m_Particles.size(), 0);

	m_Shader->unSetShader();
	m_Buffer->unsetBuffer(0);

	m_Render->unsetCameraBuffers();
}

void Particles::emitNewParticle()
{
	if (m_Particles.size() < m_MaxParticles)
	{
		Particle *p = new(*m_PoolAllocator)Particle;
		std::uniform_real_distribution<float> color(0.f, 1.f);
		std::uniform_real_distribution<float> velocity(-1.f, 1.f);

		p->position = m_EmitPosition;
		p->elapsedTime = 0.f;
		p->color = DirectX::XMFLOAT3(color(generator), color(generator), color(generator));
		p->velocity = DirectX::XMFLOAT3(velocity(generator), velocity(generator), velocity(generator));
		m_Particles.push_back(p);
	}
}

void Particles::killOldParticles()
{
	for (int i = 0; i < (int)m_Particles.size(); ++i)
	{
		if (m_Particles.at(i)->elapsedTime >= m_ParticleMaxLife)
		{
			operator delete(m_Particles.at(i), *m_PoolAllocator);
			m_Particles.erase(m_Particles.begin() + i);
			--i;
		}
	}
}


DirectX::XMVECTOR Particles::cohesion(Particle *p, int p_index)
{
	using namespace DirectX;

	int index = 0;
	XMVECTOR ret = XMVectorSet(0,0,0,0);
	for (Particle *particle : m_Particles)
	{
		if (index == p_index)
		{
			index++;
			continue;
		}
		
		ret += XMLoadFloat3(&particle->position);
		index++;
	}
	if (m_Particles.size() > 1)
		ret /= (m_Particles.size() - 1);

	return (ret - XMLoadFloat3(&p->position)) / 100;
}
DirectX::XMVECTOR Particles::alignment(Particle *p, int p_index)
{
	using namespace DirectX;

	int index = 0;
	XMVECTOR ret = XMVectorSet(0, 0, 0, 0);;
	for (Particle *particle : m_Particles)
	{
		if (index == p_index)
		{
			index++;
			continue;
		}

		ret += XMLoadFloat3(&particle->velocity);
		index++;
	}
	if (m_Particles.size() > 1)
		ret /= (m_Particles.size() - 1);

	return (ret - XMLoadFloat3(&p->velocity)) / 8;
}
DirectX::XMVECTOR Particles::separation(Particle *p, int p_index)
{
	using namespace DirectX;

	int index = 0;
	XMVECTOR ret = XMVectorSet(0, 0, 0, 0);;
	for (Particle *particle : m_Particles)
	{
		if (index == p_index)
		{
			index++;
			continue;
		}

		XMVECTOR v = XMLoadFloat3(&particle->position) - XMLoadFloat3(&p->position);
		XMVECTOR length = XMVector3Length(v);
		if (XMVectorGetX(length) < 2)
			ret -= v;
		
		index++;
	}

	return ret;
}
DirectX::XMVECTOR Particles::goal(Particle *p)
{
	using namespace DirectX;

	XMVECTOR ret = XMVectorSet(10, 5, 10, 0);

	return (ret - XMLoadFloat3(&p->position)) / 100;
}