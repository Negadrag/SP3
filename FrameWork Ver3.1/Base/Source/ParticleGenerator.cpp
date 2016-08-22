#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(GEOMETRY_TYPE meshID)
{
	this->meshID = meshID;
	f_frequency = 5.f;
	f_spawnTimer = 0.f;
	i_maxParticles = 300;
	i_particleCount = 0;
	this->pos.SetZero();
	this->scale.SetZero();
	this->minVel.SetZero();
	this->maxVel.SetZero();
	isActive = true;
	i_spawnAmount = 0;
	f_lifeTime = 0.f;
}

ParticleGenerator::ParticleGenerator(GEOMETRY_TYPE meshID, Vector3 pos, Vector3 scale, Vector3 minVel, Vector3 maxVel)
{
	this->meshID = meshID;
	this->pos = pos;
	this->scale = scale;
	this->minVel = minVel;
	this->maxVel = maxVel;
	f_frequency = 5.f;
	f_spawnTimer = 0.f;
	i_maxParticles = 300;
	i_particleCount = 0;
	isActive = true;
	i_spawnAmount = 0;
	f_lifeTime = 0.f;
}

ParticleGenerator::~ParticleGenerator()
{
	ClearParticles();
}

void ParticleGenerator::SetType(GEOMETRY_TYPE meshID)
{
	this->meshID = meshID;
}

void ParticleGenerator::SetFrequency(float f)
{
	this->f_frequency = f;
}

Particles* ParticleGenerator::GetParticle()
{
	for (std::vector<Particles*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		Particles* particle = (Particles*)(*it);
		if (!(particle->b_isActive))
		{
			particle->b_isActive = true;
			particle->lifeTime = 5.f;
			++i_particleCount;
			return particle;
		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{
		Particles* particle = new Particles(meshID);
		particleList.push_back(particle);
	}
	particleList.back()->b_isActive = true;
	particleList.back()->lifeTime = 5.f;
	return particleList.back();
}

void ParticleGenerator::Update(double dt)
{

	f_spawnTimer += (float)dt;
	if (f_spawnTimer >= 1.f / f_frequency && i_particleCount<i_maxParticles && isActive == true)
	{
		f_spawnTimer = 0.f;
		for (int i = 0; i < i_spawnAmount; ++i)
		{
			Particles* particle = GetParticle();
			particle->meshID = this->meshID;
			particle->scale = this->scale;
			particle->vel.Set(Math::RandFloatMinMax(minVel.x, maxVel.x), Math::RandFloatMinMax(minVel.y, maxVel.y), Math::RandFloatMinMax(minVel.z, maxVel.z));
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos = this->pos;
			particle->lifeTime = f_lifeTime;
			particle->f_distTravelled = 0.f;
			particle->f_maxDist = this->f_maxDist;
		}
	}
	for (std::vector<Particles*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		Particles* particle = (Particles*)(*it);
		particle->Update(dt, i_particleCount);
	}
}

void ParticleGenerator::SetCap(int cap)
{
	i_maxParticles = cap;
}

void ParticleGenerator::ClearParticles()
{
	for (vector<Particles*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		delete(*it);
	}
	particleList.clear();
}

void ParticleGenerator::SpawnParticle()
{
	if (i_particleCount<i_maxParticles)
	{
		for (int i = 0; i < i_spawnAmount; ++i)
		{
			Particles* particle = GetParticle();
			particle->meshID = this->meshID;
			particle->scale = this->scale;
			particle->vel.Set(Math::RandFloatMinMax(minVel.x, maxVel.x), Math::RandFloatMinMax(minVel.y, maxVel.y), Math::RandFloatMinMax(minVel.z, maxVel.z));
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos = this->pos;
			particle->lifeTime = f_lifeTime;
			particle->f_distTravelled = 0.f;
			particle->f_maxDist = this->f_maxDist;
		}
	}
}

void ParticleGenerator::SpawnParticle(Vector3 pos)
{
	if (i_particleCount<i_maxParticles)
	{
		for (int i = 0; i < i_spawnAmount; ++i)
		{
			Particles* particle = GetParticle();
			particle->meshID = this->meshID;
			particle->scale = this->scale;
			particle->vel.Set(Math::RandFloatMinMax(minVel.x, maxVel.x), Math::RandFloatMinMax(minVel.y, maxVel.y), Math::RandFloatMinMax(minVel.z, maxVel.z));
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos = pos;
			particle->lifeTime = f_lifeTime;
			particle->f_distTravelled = 0.f;
			particle->f_maxDist = this->f_maxDist;
		}
	}
}