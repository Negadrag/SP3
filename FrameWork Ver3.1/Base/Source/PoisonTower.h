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
	virtual bool LevelUp();

	static ESSENCE_TYPE type;
	static int cost;
	static int ecost;
	float f_PoisonDura;
	float f_PoisonSlowAmount;
	float f_PoisonDPS;
	string towerUpgrade;

private:

};

#endif