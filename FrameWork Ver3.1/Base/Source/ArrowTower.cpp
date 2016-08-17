#include "ArrowTower.h"
#include "SingleTarget.h"

ArrowTower::ArrowTower()
:Tower()
{
	this->level = T_TOWER1;
	this->meshID = GEO_ARROWTOWER;
	SetAtkDmg(10);
	SetRange(5);
	SetSpdRate(4.f);
	this->p_speed = 10.f;
	this->projectile_meshID = GEO_ARROW;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = NEAREST_ENEMY;
	upgrade = false;
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


void ArrowTower::Fire()
{
	Enemy* enemy = SearchEnemy(GetEnemyInRange());
	if (enemy == nullptr || enemy->b_isActive == false)
	{
		return;
	}
	Projectile* projectile = GetProjectile();
	projectile->meshID = this->projectile_meshID;
	projectile->pos = this->pos + heightOffset;

	if (projectile->meshID == GEO_ARROW)
		projectile->scale.Set(2, 2, 2);
	else
		projectile->scale.Set(0.5f, 0.5f, 0.5f);

	projectile->p_speed = this->p_speed;
	projectile->enemy = enemy;
	projectile->vel = (enemy->pos - projectile->pos).Normalize() * p_speed;
	projectileList.push_back(projectile);
	//enemy->i_health -= 1;
}

void ArrowTower::Update(double dt)
{
	Tower::Update(dt);
	if (upgrade)
	{
		level = T_TOWER2;
	}
	if (level == T_TOWER2)
	{
		
		SetRange(10);
	}

	std::cout << this->atkDamage << std::endl;
}