#ifndef ICETOWER_H
#define ICETOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"

class IceTower : public Tower
{
public:
	IceTower();
	~IceTower();

	ParticleGenerator particleGenerator;

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual void LevelUp();

	static int cost;

private:
};

#endif