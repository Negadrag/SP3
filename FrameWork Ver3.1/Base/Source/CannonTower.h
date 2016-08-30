#ifndef CANNONTOWER_H
#define CANNONTOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"

class CannonTower : public Tower
{
public:
	CannonTower();
	~CannonTower();

	ParticleGenerator particleGenerator;

	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();
	virtual void UpdateMesh();


	static ESSENCE_TYPE type;
	static int cost;
	static int ecost;
private:

};

#endif