#ifndef SINGLETARGET_H
#define SINGLETARGET_H

#include "Vector3.h"
#include "Renderable.h"
#include "Enemy.h"
#include "Projectile.h"

class SingleTarget :public Projectile
{
public:
	SingleTarget(GEOMETRY_TYPE meshID);
	~SingleTarget();

	virtual void Update(double dt);
private:


};

#endif