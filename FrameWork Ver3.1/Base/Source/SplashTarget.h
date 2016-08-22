#ifndef SPLASHTARGET_H
#define SPLASHTARGET_H

#include "Vector3.h"
#include "Renderable.h"
#include "Enemy.h"
#include "Projectile.h"

class SplashTarget :public Projectile
{
public:
	SplashTarget(GEOMETRY_TYPE meshID);
	~SplashTarget();

	vector<Enemy*>* enemyVec;

	float f_range;

	virtual void Update(double dt);
private:


};

#endif