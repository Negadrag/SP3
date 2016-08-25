#include "CannonTower.h"
#include "SplashTarget.h"

int CannonTower::cost = 15;
int CannonTower::ecost = 0;
Tower::ESSENCE_TYPE CannonTower::type = Tower::ESSENCE_TYPE::E_BASIC;

CannonTower::CannonTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(25);
	SetRange(7.f);
	SetSpdRate(0.75f);
	this->p_speed = 12.f;
	this->towerCost = cost;
	this->essenceCost = ecost;
	this->essence = type;
	this->meshID = GEO_CANNONTOWER;
	this->fullMeshID = GEO_CANNONTOWER;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = LOWEST_HEALTH;
	s_name = "Cannon Tower";

	this->particleGenerator.SetType(GEO_CANNONBLAST);
	this->particleGenerator.SetFrequency(25);
	this->particleGenerator.SetCap(1000);
	this->particleGenerator.f_lifeTime = 1.f;
	this->particleGenerator.minVel.Set(-2.f,-2.f,0.f);
	this->particleGenerator.maxVel.Set(2.f, 2.f, 0.f);
	this->particleGenerator.scale.Set(0.15f,0.15f,0.15f);
	this->particleGenerator.i_particleCount = 0;
	this->particleGenerator.f_maxDist = 3.f;
	this->particleGenerator.isActive = false;
	this->particleGenerator.i_spawnAmount = 15;

	upgrades[0] = "Ice";
	upgrades[1] = "Mortar";
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


bool CannonTower::LevelUp()
{
	if (this->i_level <= 2)
	{
		this->i_level++;
		this->atkDamage += 10;
		return true;
	}
	return false;
}