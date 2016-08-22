#ifndef ARROWTOWER_H
#define ARROWTOWER_H

#include "Tower.h"

class ArrowTower : public Tower
{
public:
	ArrowTower();
	~ArrowTower();

	virtual Projectile* GetProjectile();

	bool upgrade;

	static int cost;

private:

};

#endif