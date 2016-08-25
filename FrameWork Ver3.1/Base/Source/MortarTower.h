#ifndef MORTARTOWER_H
#define MORTARTOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"

class MortarTower : public Tower
{
public:
	MortarTower();
	~MortarTower();

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();

	ParticleGenerator particleGenerator;
	static int cost;

private:

};

#endif