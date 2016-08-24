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
	virtual bool LevelUp();
	virtual void Fire(double dt);

	string towerUpgrade;
	static int cost;
	Renderable child2;
	Renderable child3;

private:

};

#endif