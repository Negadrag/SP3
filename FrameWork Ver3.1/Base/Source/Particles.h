#ifndef PARTICLES_H
#define PARTICLES_H

#include "Vector3.h"
#include "Renderable.h"

class Particles :public Renderable
{
public:
	Particles(GEOMETRY_TYPE ID = GEO_NULL);
	~Particles();

	float f_distTravelled;
	float f_maxDist;
	Vector3 vel;
	float rotation;
	float rotationSpeed;
	float lifeTime;
	virtual void Update(double dt, int& particle_count);
private:

};

#endif