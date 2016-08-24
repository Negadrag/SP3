#ifndef MORTARTOWER_H
#define MORTARTOWER_H

#include "Tower.h"

class MortarTower : public Tower
{
public:
	MortarTower();
	~MortarTower();

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();

	static int cost;

private:

};

#endif