#include "CannonTower.h"
#include "SplashTarget.h"

CannonTower::CannonTower()
:Tower()
{
	this->meshID = GEO_CANNONTOWER;
	SetAtkDmg(10);
	SetRange(30);
	SetSpdRate(0.5f);
	this->p_speed = 5.f;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = FURTHER_ENEMY;
}

Projectile* CannonTower::GetProjectile()
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

		Projectile* projectile = new SplashTarget(projectile_meshID);
		projectile->b_isActive = false;
		projectileList.push_back(projectile);
	}
	projectileList.back()->b_isActive = true;
	return projectileList.back();
}

void CannonTower::Fire()
{
	Enemy* enemy = SearchEnemy(GetEnemyInRange());
	if (enemy == nullptr || enemy->b_isActive == false)
	{
		return;
	}
	Projectile* projectile = GetProjectile();
	projectile->meshID = this->projectile_meshID;
	projectile->pos = this->pos + heightOffset;
	projectile->i_damage = this->atkDamage;

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

CannonTower::~CannonTower()
{

}

void CannonTower::Update(double dt)
{
	Tower::Update(dt);

}