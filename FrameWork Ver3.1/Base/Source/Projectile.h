#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector3.h"
#include "Renderable.h"
#include "Enemy.h"

class Projectile :public Renderable
{
public:
	Projectile(GEOMETRY_TYPE ID = GEO_NULL);
	Projectile(Vector3 pos, Vector3 scale);
	~Projectile();


	Vector3 vel;
	float p_speed;
	Enemy *enemy;

	virtual void Update(double dt);
private:

	
};

#endif