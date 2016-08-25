#ifndef MORTARTOWER_H
#define MORTARTOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"
#include "FixedProjectile.h"

class MortarTower : public Tower
{
public:
	MortarTower();
	~MortarTower();

	virtual FixedProjectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();
	virtual void Fire(double dt);

	ParticleGenerator particleGenerator;
	static int cost;

private:

};

#endif