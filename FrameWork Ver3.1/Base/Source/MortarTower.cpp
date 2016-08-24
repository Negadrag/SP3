#include "MortarTower.h"
#include "SplashTarget.h"

int MortarTower::cost = 5;

MortarTower::MortarTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(10);
	SetRange(30);
	SetSpdRate(0.5f);
	this->p_speed = 5.f;
	this->towerCost = cost;
	this->meshID = GEO_MORTARBASE;
	this->fullMeshID = GEO_MORTARTOWER;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = LOWEST_HEALTH;
	s_name = "Mortar Tower";
	child.meshID = GEO_MORTARCANNON;
}

Projectile* MortarTower::GetProjectile()
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


MortarTower::~MortarTower()
{

}

void MortarTower::Update(double dt)
{
	Tower::Update(dt);
}


bool MortarTower::LevelUp()
{
	if (this->i_level <= 2)
	{
		this->i_level++;
		this->atkDamage += 5;
		this->atkRange += 1;
		if (atkRange > 7)
		{
			atkRange = 7;
		}
		return true;
	}
	return false;
}