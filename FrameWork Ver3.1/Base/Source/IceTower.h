#ifndef ICETOWER_H
#define ICETOWER_H

#include "Tower.h"

class IceTower : public Tower
{
public:
	IceTower();
	~IceTower();

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual void LevelUp();

private:

};

#endif