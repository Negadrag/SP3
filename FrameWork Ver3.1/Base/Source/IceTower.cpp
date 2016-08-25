#include "IceTower.h"
#include "SplashTarget.h"
#include "IceProjectile.h"

int IceTower::cost = 5;

IceTower::IceTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(10);
	SetRange(2);
	SetSpdRate(0.5f);
	this->p_speed = 5.f;
	this->f_SlowAmount = 50.f;
	this->f_SlowDura = 10.f;
	this->towerCost = cost;
	this->pos.SetZero();
	this->meshID = GEO_ICEBASE;
	this->fullMeshID = GEO_ICETOWER;
	this->projectile_meshID = GEO_ICESHOT;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = NEAREST_ENEMY;
	s_name = "Ice Tower";
	this->b_rotateWhenFire = false;
	child.meshID = GEO_ICE;
	child.pos.Set(0, 0, 1.2f);

	particleGenerator.SetType(GEO_ICEPARTICLE);
	particleGenerator.SetFrequency(20);
	particleGenerator.SetCap(1000);
	particleGenerator.i_spawnAmount = 30;
	particleGenerator.f_lifeTime = 0.3f;
	particleGenerator.minVel.Set(0, 0, 0);
	particleGenerator.maxVel.Set(0, 0, 0);
	particleGenerator.scale.Set(0.07f, 0.07f, 0.07f);
	particleGenerator.i_particleCount = 0;
	particleGenerator.f_maxDist = 2.f;
	particleGenerator.isActive = false;
	particleGenerator.i_spawnAmount = 10;

}

Projectile* IceTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		IceProjectile* projectile = (IceProjectile*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
			projectile->enemyVec = this->enemyList;
			projectile->iceparticle = &(particleGenerator);
			projectile->f_slowAmount = this->f_SlowAmount;
			projectile->f_slowDuration = this->f_SlowDura;
			return projectile;

		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{

		IceProjectile* projectile = new IceProjectile(projectile_meshID);
		projectile->b_isActive = false;
		projectileList.push_back(projectile);
	}

	return GetProjectile();
}


IceTower::~IceTower()
{

}

void IceTower::Update(double dt)
{
	Tower::Update(dt);
	particleGenerator.Update(dt);
	child.rotation.z += 90.f * dt;
	if (child.rotation.z > 360.f)
		child.rotation.z -= 360.f;
}


bool IceTower::LevelUp()
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