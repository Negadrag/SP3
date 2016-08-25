#ifndef ARROWTOWER_H
#define ARROWTOWER_H

#include "Tower.h"

class ArrowTower : public Tower
{
public:
	ArrowTower();
	~ArrowTower();

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();


	string towerUpgrade;
	static int cost;

private:

};

#endif