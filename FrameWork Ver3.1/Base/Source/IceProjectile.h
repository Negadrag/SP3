#ifndef ICEPROJECTILE_H
#define ICEPROJECTILE_H

#include "SplashTarget.h"

class IceProjectile :public SplashTarget
{
public:
	IceProjectile(GEOMETRY_TYPE meshID);
	~IceProjectile();

	virtual void Update(double dt);

	float f_slowDuration;
	float f_slowAmount;
private:


};

#endif