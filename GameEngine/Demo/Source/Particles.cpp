#include "Particles.h"
#include "Utilities.h"

#include <algorithm>

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
	DirectX::XMFLOAT3 p_EmitPosition, float p_MaxLife, float p_TimePerParticle)
{
	m_PoolAllocator = new Allocator::PoolAllocator(sizeof(Particle), p_MaxParticles + 1);

	m_EmitPosition = p_EmitPosition;
	m_ParticleMaxLife = p_MaxLife;
	m_MaxParticles = p_MaxParticles;
	m_TimePerParticle = p_TimePerParticle;

	//color = std::uniform_real_distribution<float>(0.f, 1.f);
	//velocity = std::uniform_real_distribution<float>(-1.f, 1.f);
}

void Particles::update(float p_Dt)
{
	m_AccumulatedTime += p_Dt;
	if (m_AccumulatedTime > m_TimePerParticle)
	{
		m_AccumulatedTime = 0.f;

		emitNewParticle();
	}
	for (Particle *p : m_Particles)
	{
		p->elapsedTime += p_Dt;
	}
	killOldParticles();
}

void Particles::render()
{

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
	//std::vector<Particle*>::iterator removeIt = std::remove_if(m_Particles.begin(), m_Particles.end(), [&](Particle *p)
	//{
	//	return p->elapsedTime >= m_ParticleMaxLife;
	//});

	//for (unsigned int i = std::distance(m_Particles.begin(), removeIt); i < m_Particles.size(); ++i)
	//{
	//	operator delete(m_Particles.at(i), *m_PoolAllocator);
	//	//*it = nullptr;
	//}
	//if (removeIt != m_Particles.end())
	//	m_Particles.erase(removeIt, m_Particles.end());

	for (int i = 0; i < m_Particles.size(); ++i)
	{
		if (m_Particles.at(i)->elapsedTime >= m_ParticleMaxLife)
		{
			operator delete(m_Particles.at(i), *m_PoolAllocator);
			m_Particles.erase(m_Particles.begin() + i);
			--i;
		}
	}
}