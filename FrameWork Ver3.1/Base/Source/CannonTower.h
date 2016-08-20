#ifndef CANNONTOWER_H
#define CANNONTOWER_H

#include "Tower.h"

class CannonTower : public Tower
{
public:
	CannonTower();
	~CannonTower();

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);

	static int cost;

private:

};

#endif