#include "CannonTower.h"
#include "SplashTarget.h"

int CannonTower::cost = 8;

CannonTower::CannonTower()
:Tower()
{
	this->i_level = 1;
	this->meshID = GEO_CANNONTOWER;
	SetAtkDmg(10);
	SetRange(5.f);
	SetSpdRate(0.5f);
	this->p_speed = 12.f;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = LOWEST_HEALTH;
	s_name = "Cannon Tower";

	this->particleGenerator.SetType(GEO_ICEPARTICLE);
	this->particleGenerator.SetFrequency(25);
	this->particleGenerator.SetCap(1000);
	this->particleGenerator.f_lifeTime = 0.3f;
	this->particleGenerator.minVel.Set(0, 0, 0);
	this->particleGenerator.maxVel.Set(0, 0, 0);
	this->particleGenerator.scale.Set(0.07f, 0.07f, 0.07f);
	this->particleGenerator.i_particleCount = 0;
	this->particleGenerator.f_maxDist = 2.f;
	this->particleGenerator.isActive = false;
	this->particleGenerator.i_spawnAmount = 5;
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
			projectile->iceparticle = &particleGenerator;
			return projectile;

		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{

		SplashTarget* projectile = new SplashTarget(projectile_meshID);
		projectile->b_isActive = false;
		projectile->iceparticle = &particleGenerator;
		projectileList.push_back(projectile);
	}
	
	return GetProjectile();
}


CannonTower::~CannonTower()
{
	particleGenerator.ClearParticles();
}

void CannonTower::Update(double dt)
{
	Tower::Update(dt);
	particleGenerator.Update(dt);

}


void CannonTower::LevelUp()
{
	if (this->i_level >= 2)
	{
		i_level = 2;
	}
	this->i_level++;
	this->atkDamage += 5;
	this->atkRange += 1;
	if (atkRange > 7)
	{
		atkRange = 7;
	}
}