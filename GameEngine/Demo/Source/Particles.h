#pragma once
#include <DirectXMath.h>
#include <vector>
#include <random>

#include "IAllocators.h"

class Particles
{
public:
	struct Particle
	{
		float elapsedTime;
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 velocity;
	};

private:
	DirectX::XMFLOAT3 m_EmitPosition;

	float m_TimePerParticle;
	float m_AccumulatedTime;
	float m_ParticleMaxLife;
	unsigned int m_MaxParticles;

	
	std::vector<Particle*> m_Particles;

	Allocator::PoolAllocator *m_PoolAllocator;
	std::default_random_engine generator;

	

public:
	Particles();
	~Particles();

	void initialize(char *p_Buffer, unsigned int p_MaxParticles, 
		DirectX::XMFLOAT3 p_EmitPosition, float p_MaxLife, float p_TimePerParticle);

	void update(float p_Dt);
	void render();

private:
	void emitNewParticle();
	void killOldParticles();
};

