#ifndef CAPTURETOWER_H
#define CAPTURETOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"


class CaptureTower : public Tower
{
public:
	CaptureTower();
	~CaptureTower();

	ParticleGenerator particleGenerator;

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual void LevelUp();
	virtual void Fire(double dt);

	static int cost;


private:

};

#endif