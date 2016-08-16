#ifndef CANNONTOWER_H
#define CANNONTOWER_H

#include "Tower.h"

class CannonTower : public Tower
{
public:
	CannonTower();
	~CannonTower();

	virtual void Update(double dt);

private:

};

#endif