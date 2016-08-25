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
	this->fullMeshID = GEO_MORTARCANNON;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = LOWEST_HEALTH;
	s_name = "Mortar Tower";
	child.meshID = GEO_MORTARCANNON;

	this->particleGenerator.SetType(GEO_CANNONBLAST);
	this->particleGenerator.SetFrequency(25);
	this->particleGenerator.SetCap(1000);
	this->particleGenerator.f_lifeTime = 1.f;
	this->particleGenerator.minVel.Set(-2.f, -2.f, 0.f);
	this->particleGenerator.maxVel.Set(2.f, 2.f, 0.f);
	this->particleGenerator.scale.Set(0.15f, 0.15f, 0.15f);
	this->particleGenerator.i_particleCount = 0;
	this->particleGenerator.f_maxDist = 3.f;
	this->particleGenerator.isActive = false;
	this->particleGenerator.i_spawnAmount = 15;
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


MortarTower::~MortarTower()
{

}

void MortarTower::Update(double dt)
{
	Tower::Update(dt);
	particleGenerator.Update(dt);
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