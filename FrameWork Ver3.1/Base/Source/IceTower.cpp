#include "IceTower.h"
#include "SplashTarget.h"

int IceTower::cost = 5;

IceTower::IceTower()
:Tower()
{
	this->i_level = 1;
	this->pos.SetZero();
	this->meshID = GEO_ICEBASE;
	SetAtkDmg(10);
	SetRange(30);
	SetSpdRate(0.5f);
	this->p_speed = 5.f;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = FIRST_ENEMY;
	s_name = "Ice Tower";
	this->b_rotateWhenFire = false;
	child.meshID = GEO_ICECRYSTAL;

	this->iceparticle.SetType(GEO_ICEPARTICLE);
	this->iceparticle.SetFrequency(25);
	this->iceparticle.SetCap(1000);
	this->iceparticle.f_lifeTime = 1.f;
	this->iceparticle.minVel.Set(-2.f, -2.f, 0.f);
	this->iceparticle.maxVel.Set(2.f, 2.f, 0.f);
	this->iceparticle.scale.Set(0.1f, 0.1f, 0.1f);
	this->iceparticle.i_particleCount = 0;
	this->iceparticle.f_maxDist = 3.f;
	this->iceparticle.isActive = false;
	this->iceparticle.i_spawnAmount = 15;

}

Projectile* IceTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		SplashTarget* projectile = (SplashTarget*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
			projectile->enemyVec = this->enemyList;
			projectile->iceparticle = &(this->iceparticle);
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


IceTower::~IceTower()
{

}

void IceTower::Update(double dt)
{
	Tower::Update(dt);

	child.rotation.z += 90.f * dt;
	if (child.rotation.z > 360.f)
		child.rotation.z -= 360.f;

}


void IceTower::LevelUp()
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