#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include "Particles.h"
#include <vector>
#include "Renderable.h"
using std::vector;

class ParticleGenerator
{
public:
	ParticleGenerator(GEOMETRY_TYPE meshID = GEO_NULL);
	ParticleGenerator(GEOMETRY_TYPE meshID, Vector3 pos, Vector3 scale, Vector3 minVel, Vector3 maxVel);
	~ParticleGenerator();
	
	void SetType(GEOMETRY_TYPE meshID);
	void SetFrequency(float f);
	Particles* GetParticle();
	void Update(double dt);
	void SetCap(int cap);
	void ClearParticles();

	bool isActive;
	vector<Particles*> particleList;

	Vector3 pos;
	Vector3 scale;
	Vector3 minVel;
	Vector3 maxVel;
private:
	GEOMETRY_TYPE meshID;
	float f_frequency;
	float f_spawnTimer;
	int i_particleCount;
	int i_maxParticles;
	
};

#endif