#pragma once
#include "IAllocators.h"

#include <DirectXMath.h>
#include <vector>
#include <random>

class Render;
class Shader;
class Buffer;

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

	struct renderParticle
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
	};

	DirectX::XMFLOAT3 m_EmitPosition;

	float m_TimePerParticle;
	float m_AccumulatedTime;
	float m_ParticleMaxLife;
	unsigned int m_MaxParticles;

	std::vector<Particle*> m_Particles;

	Allocator::PoolAllocator *m_PoolAllocator;
	std::default_random_engine generator;

	Render *m_Render;
	Shader *m_Shader;
	Buffer *m_Buffer;

public:
	Particles(void);
	~Particles(void);

	/**
	* Initialized a Particle emitter.
	* @param p_Buffer, pointer to a memory block for the pool allocator to use.
	* @param p_MaxParticles, max number of particles to create
	* @param p_EmitPosition, the position in world coordinates which new particles will be created from.
	* @param p_MaxLife, the lifespan of a particle in seconds.
	* @param p_TimePerParticle, the time between particle creations in seconds.
	* @param p_Render, pointer to Render. Cannot be nullptr.

	*/
	void initialize(char *p_Buffer, unsigned int p_MaxParticles, 
		DirectX::XMFLOAT3 p_EmitPosition, float p_MaxLife, float p_TimePerParticle, Render *p_Render);

	/**
	* Updates each particle.
	* @param p_Dt, the programs deltatime in seconds.
	*/
	void update(float p_Dt);

	/**
	* Renders the particles using Render.
	*/
	void render(void);
private:
	void emitNewParticle(void);
	void killOldParticles(void);

	/**
	* Boid functions
	*/
	DirectX::XMVECTOR cohesion(Particle *p_Particle, int p_index);
	DirectX::XMVECTOR alignment(Particle *p_Particle, int p_index);
	DirectX::XMVECTOR separation(Particle *p_Particle, int p_index);
	DirectX::XMVECTOR goal(Particle *p_Particle);
};

