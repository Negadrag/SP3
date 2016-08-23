#ifndef SINGLETARGET_H
#define SINGLETARGET_H

#include "Vector3.h"
#include "Renderable.h"
#include "Enemy.h"
#include "Projectile.h"
#include "ParticleGenerator.h"

class SingleTarget :public Projectile
{
public:
	SingleTarget(GEOMETRY_TYPE meshID);
	~SingleTarget();

	vector<Enemy*>* enemyVec;

	float f_range;
	ParticleGenerator* iceparticle;
	float f_particleSpawnTimer;
	virtual void Update(double dt);
private:


};

#endif