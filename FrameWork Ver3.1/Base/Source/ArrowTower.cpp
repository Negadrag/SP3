#include "ArrowTower.h"
#include "SingleTarget.h"

ArrowTower::ArrowTower()
:Tower()
{
	//this->level = T_TOWER1;
	this->meshID = GEO_ARROWTOWER;
	SetAtkDmg(10);
	SetRange(5);
	SetSpdRate(2.f);
	this->p_speed = 10.f;
	this->projectile_meshID = GEO_ARROW;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = NEAREST_ENEMY;
	upgrade = false;
	s_name = "Arrow Tower";
}

ArrowTower::~ArrowTower()
{

}


Projectile* ArrowTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
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
	return projectileList.back();
}
