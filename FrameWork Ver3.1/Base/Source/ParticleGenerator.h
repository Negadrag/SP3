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

	void SpawnParticle();
	void SpawnParticle(Vector3 pos);

	bool isActive;
	vector<Particles*> particleList;


	float f_frequency;
	Vector3 pos;
	Vector3 scale;
	Vector3 minVel;
	Vector3 maxVel;
	float f_maxDist;
	int i_particleCount;
	float f_lifeTime;
	int i_spawnAmount;//number to spawn per time
private:
	GEOMETRY_TYPE meshID;
	
	float f_spawnTimer;
	//int i_particleCount;
	int i_maxParticles;
	
	

};

#endif