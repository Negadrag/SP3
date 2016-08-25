#ifndef SPEEDTOWER_H
#define SPEEDTOWER_H

#include "Tower.h"

class SpeedTower : public Tower
{
public:
	SpeedTower();
	~SpeedTower();

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();

	static ESSENCE_TYPE type;
	static int cost;
	static int ecost;
private:

};

#endif