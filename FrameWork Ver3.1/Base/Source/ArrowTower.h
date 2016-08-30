/****************************************************************************/
/*!
\file ArrowTower.h
\author Matsuda Kenichi
\par email: blackdarkrose91@gmail.com
\brief
Class to define arrow tower
*/
/****************************************************************************/
#ifndef ARROWTOWER_H
#define ARROWTOWER_H

#include "Tower.h"

/****************************************************************************/
/*!
Class ArrowTower:
Public Tower:
\brief Defines arrowtower and it method
*/
/****************************************************************************/
class ArrowTower : public Tower
{
public:
	ArrowTower();
	~ArrowTower();

	virtual Projectile* GetProjectile();	//Get Projectile
	virtual void Update(double dt);
	virtual bool LevelUp();					//Level up function

	
	string towerUpgrade;

	static ESSENCE_TYPE type;
	static int cost;	//Coin Cost
	static int ecost;	//Essence Cost

private:

};

#endif