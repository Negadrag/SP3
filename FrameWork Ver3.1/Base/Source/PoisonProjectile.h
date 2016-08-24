#ifndef POISONPROJECTILE_H
#define POISONPROJECTILE_H

#include "SingleTarget.h"

class PoisonProjectile :public SingleTarget
{
public:
	PoisonProjectile(GEOMETRY_TYPE meshID);
	~PoisonProjectile();

	virtual void Update(double dt);

	float f_poisondmg;
	float f_slowAmount;
	float f_poisondura;
private:


};

#endif