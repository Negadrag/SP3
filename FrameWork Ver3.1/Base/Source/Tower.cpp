#include "Tower.h"

Tower::Tower()
: Renderable()
{
	p_spawnTimer = 0.f;
	towerCost = 0.f;
	atkDamage = 0.f;
	atkSpeed = 0.f;
	atkRange = 0.f;
	p_speed = 10.0f;
	p_projectileCount = 0;
	p_maxProjectile = 0;
	this->pos.SetZero();

	heightOffset.SetZero();
}

Tower::Tower(Vector3 pos, Vector3 scale, Vector3 heightOffset)
: Renderable()
{
	p_spawnTimer = 0.f;
	this->pos = pos;
	this->scale = scale;
	towerCost = 0.f;
	atkDamage = 0.f;
	atkSpeed = 0.f;
	atkRange = 0.f;
	p_projectileCount = 0;
	p_maxProjectile = 0;
	heightOffset.Set(0, 0, 5);
	p_speed = 10.0f;
	
}

Tower::~Tower()
{
	for (vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		delete(*it);
	}
}

void Tower::SetType(GEOMETRY_TYPE meshID)
{
	this->meshID = meshID;
}

void Tower::SetCost(float c)
{
	this->towerCost = c;
}

void Tower::SetAtkDmg(float ad)
{
	this->atkDamage = ad;
}

void Tower::SetSpdRate(float sr)
{
	this->atkSpeed = sr;
}

void Tower::SetRange(float r)
{
	this->atkRange = r;
}

Projectile* Tower::GetProjectile()
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
		Projectile* projectile = new Projectile(projectile_meshID);
		projectile->b_isActive = false;
		projectileList.push_back(projectile);
	}
	projectileList.back()->b_isActive = true;
	return projectileList.back();
}

void Tower::Update(double dt)
{

	p_spawnTimer += (float)dt;
	if (p_spawnTimer >= 1.f /this->atkSpeed)// p_frequency) && p_projectileCount<p_maxProjectile)
	{
		p_spawnTimer = 0.f;
		Fire();
	}
	/*for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)(*it);
		projectile->Update(dt);
	}*/

	
}

void Tower::Fire()
{
	if (enemy->b_isActive == false)
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
	projectile->enemy = this->enemy;
	projectile->vel = (enemy->pos - projectile->pos).Normalize() * p_speed;
	projectileList.push_back(projectile);

	
}

void Tower::ClearProjectile()
{
	for (vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		delete(*it);
	}
	projectileList.clear();
}