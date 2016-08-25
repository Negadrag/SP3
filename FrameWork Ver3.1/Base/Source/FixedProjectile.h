#ifndef FIXEDPROJECTILE_H
#define FIXEDPROJECTILE_H

#include "SplashTarget.h"

class FixedProjectile :public SplashTarget
{
public:
	FixedProjectile(GEOMETRY_TYPE meshID);
	~FixedProjectile();

	virtual void Update(double dt);

	Vector3 enemyLastPos;
private:


};

#endif