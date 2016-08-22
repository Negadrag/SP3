#ifndef CANNONTOWER_H
#define CANNONTOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"

class CannonTower : public Tower
{
public:
	CannonTower();
	~CannonTower();

	ParticleGenerator particleGenerator;

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual void LevelUp();

	static int cost;
private:

};

#endif