#ifndef POISONTOWER_H
#define POISONTOWER_H

#include "Tower.h"

class PoisonTower : public Tower
{
public:
	PoisonTower();
	~PoisonTower();

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual void LevelUp();

private:

};

#endif