#include "CannonTower.h"
#include "SplashTarget.h"

int CannonTower::cost = 8;

CannonTower::CannonTower()
:Tower()
{
	this->meshID = GEO_CANNONTOWER;
	SetAtkDmg(10);
	SetRange(5.f);
	SetSpdRate(0.5f);
	this->p_speed = 5.f;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = LOWEST_HEALTH;
	s_name = "Cannon Tower";
}

Projectile* CannonTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		SplashTarget* projectile = (SplashTarget*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
			projectile->enemyVec = this->enemyList;
			return projectile;

		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{

		SplashTarget* projectile = new SplashTarget(projectile_meshID);
		projectile->b_isActive = false;
		projectileList.push_back(projectile);
	}
	
	return GetProjectile();
}


CannonTower::~CannonTower()
{

}

