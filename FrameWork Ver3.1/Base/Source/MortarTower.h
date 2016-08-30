#ifndef MORTARTOWER_H
#define MORTARTOWER_H

#include "Tower.h"
#include "ParticleGenerator.h"
#include "FixedProjectile.h"

class MortarTower : public Tower
{
public:
	MortarTower();
	~MortarTower();

	virtual FixedProjectile* GetProjectile();
	virtual void Update(double dt);
	virtual bool LevelUp();
	virtual void Fire(double dt);
	virtual void UpdateMesh();


	ParticleGenerator particleGenerator;
	static ESSENCE_TYPE type;
	static int cost;
	static int ecost;

private:

};

#endif