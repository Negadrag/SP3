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

	bool upgrade;


private:

};

#endif