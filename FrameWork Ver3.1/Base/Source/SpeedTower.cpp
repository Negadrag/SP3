#include "SpeedTower.h"
#include "SingleTarget.h"

int SpeedTower::cost = 50;
int SpeedTower::ecost = 100;
Tower::ESSENCE_TYPE SpeedTower::type = Tower::ESSENCE_TYPE::E_SPEED;
SpeedTower::SpeedTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(5);
	SetRange(5);
	SetSpdRate(7);
	this->p_speed = 10.f;
	this->towerCost = cost;
	this->essenceCost = 0;
	this->essence = type;
	this->meshID = GEO_SPEEDTOWER;
	this->fullMeshID = GEO_SPEEDMODEL;
	this->projectile_meshID = GEO_BULLET;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = NEAREST_ENEMY;
	s_name = "Speed Tower";
	child.meshID = GEO_SPEED;
	child.pos.Set(0, 0, 2.2f);
}

SpeedTower::~SpeedTower()
{

}


Projectile* SpeedTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
			Music::GetInstance()->PlayMusic(5, false, 0.15f);
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
	Music::GetInstance()->PlayMusic(5, false, 0.15f);
	return projectileList.back();
}

void SpeedTower::Update(double dt)
{
	Tower::Update(dt);

}

bool SpeedTower::LevelUp()
{
	if (this->i_level <= 2)
	{
		this->i_level++;
		this->atkDamage += 1;
		if (i_level == 2)
		{
			this->atkSpeed = 8;
		}
		if (i_level == 3)
		{
			this->atkSpeed = 10;
		}
		return true;
	}
	return false;
}