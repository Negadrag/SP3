/****************************************************************************/
/*!
\file ArrowTower.cpp
\author Matsuda Kenichi
\par email: blackdarkrose91@gmail.com
*/
/****************************************************************************/
#include "ArrowTower.h"
#include "SingleTarget.h"


int ArrowTower::cost = 10;
int ArrowTower::ecost = 0;
Tower::ESSENCE_TYPE ArrowTower::type = Tower::ESSENCE_TYPE::E_BASIC;

/****************************************************************************/
/*!
\brief
	Constructor of Arrow Tower
*/
/****************************************************************************/
ArrowTower::ArrowTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(10);
	SetRange(5);
	SetSpdRate(2.f);
	this->p_speed = 10.f;
	this->towerUpgradeCost = cost;
	this->essenceUpgradeCost = ecost;
	this->essence = type;
	this->meshID = GEO_ARROWTOWER;
	this->fullMeshID = GEO_ARROWTOWER;
	this->projectile_meshID = GEO_ARROW;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = FIRST_ENEMY;
	s_name = "Arrow Tower";

	upgrades[0] = "Poison";
	upgrades[1] = "Speed";
}

/****************************************************************************/
/*!
\brief
	Destructor of Arrow Tower
*/
/****************************************************************************/
ArrowTower::~ArrowTower()
{

}

/****************************************************************************/
/*!
\brief
	Return a vector of projectile
\return
	Resulting the projectile
*/
/****************************************************************************/
Projectile* ArrowTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
			Music::GetInstance()->PlayMusic(0, false, 0.15f);
			return projectile;
		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{
		Projectile* projectile = new SingleTarget(projectile_meshID);
		projectile->b_isActive = false;
		projectileList.push_back(projectile);
	}
	projectileList.back()->b_isActive = true;
	Music::GetInstance()->PlayMusic(0, false, 0.15f);
	return projectileList.back();
}

void ArrowTower::Update(double dt)
{
	Tower::Update(dt);
}

/****************************************************************************/
/*!
\brief
	Return result of tower if upgraded
\return
	Resulting upgraded tower
*/
/****************************************************************************/
bool ArrowTower::LevelUp()
{
	if (this->i_level <= 2)
	{
		this->i_level++;
		this->atkDamage += 8;
		this->atkSpeed += 0.5f;

		if (i_level == 3)
		{
			atkRange = 7;
		}
		return true;
	}
	return false;

	
}

void ArrowTower::UpdateMesh()
{
	if (b_isFrozen == true)
	{
		meshID = GEO_ARROWTOWER_FROST;
	}
	else
	{
		meshID = GEO_ARROWTOWER;
	}
}