#include "PoisonTower.h"
#include "SingleTarget.h"
#include "PoisonProjectile.h"

int PoisonTower::cost = 5;

PoisonTower::PoisonTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(10);
	SetRange(5);
	SetSpdRate(0.5f);
	this->p_speed = 10.f;
	this->f_PoisonDPS = 1.f;
	this->f_PoisonDura = 5.f;
	this->f_PoisonSlowAmount = 25.f;
	this->towerCost = cost;
	this->meshID = GEO_POISONBASE;
	this->fullMeshID = GEO_POISONTOWER;
	this->projectile_meshID = GEO_POISONARROW;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = FIRST_ENEMY;
	s_name = "Poison Tower";
	child.meshID = GEO_BASIC;
	child.pos.Set(0, 0, 2.2f);
}

PoisonTower::~PoisonTower()
{

}


Projectile* PoisonTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		PoisonProjectile* projectile = (PoisonProjectile*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
			projectile->f_poisondmg = this->f_PoisonDPS;
			projectile->f_poisondura = this->f_PoisonDura;
			projectile->f_slowAmount = this->f_PoisonSlowAmount;
			return projectile;

		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{

		PoisonProjectile* projectile = new PoisonProjectile(projectile_meshID);
		projectile->b_isActive = false;
		projectileList.push_back(projectile);
	}
	projectileList.back()->b_isActive = true;
	return projectileList.back();
}

void PoisonTower::Update(double dt)
{
	Tower::Update(dt);
}

bool PoisonTower::LevelUp()
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