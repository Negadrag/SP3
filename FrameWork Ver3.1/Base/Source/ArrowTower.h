#ifndef ARROWTOWER_H
#define ARROWTOWER_H

#include "Tower.h"

class ArrowTower : public Tower
{
public:
	ArrowTower();
	~ArrowTower();

	virtual void Update(double dt);

private:

};

#endif