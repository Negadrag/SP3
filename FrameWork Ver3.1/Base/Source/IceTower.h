#ifndef ICETOWER_H
#define ICETOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"
#include "IceProjectile.h"

class IceTower : public Tower
{
public:
	IceTower();
	~IceTower();

	ParticleGenerator particleGenerator;

	virtual IceProjectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();
	virtual void Fire(double dt);

	static int cost;
	float f_SlowDura;
	float f_SlowAmount;

private:
};

#endif