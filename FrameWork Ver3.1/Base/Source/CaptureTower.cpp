#include "CaptureTower.h"
#include "SingleTarget.h"

int CaptureTower::cost = 5;

CaptureTower::CaptureTower()
:Tower()
{
	this->i_level = 1;
	this->meshID = GEO_ARROWTOWER;
	SetAtkDmg(10);
	SetRange(5);
	SetSpdRate(10.f);
	this->p_speed = 15.f;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = FIRST_ENEMY;
	s_name = "Capture Tower";
	this->b_rotateWhenFire = false;

	particleGenerator.SetType(GEO_ICEPARTICLE);
	particleGenerator.SetFrequency(1);
	particleGenerator.SetCap(1000);
	particleGenerator.i_spawnAmount = 10;
	particleGenerator.f_lifeTime = 2.f;
	particleGenerator.minVel.Set(0, 0, 0);
	particleGenerator.maxVel.Set(0, 0, 0);
	particleGenerator.scale.Set(0.2f, 0.2f, 0.2f);
	//particleGenerator.i_particleCount = 0;
	particleGenerator.f_maxDist = 5.f;
	particleGenerator.isActive = false;
}

CaptureTower::~CaptureTower()
{

}


Projectile* CaptureTower::GetProjectile()
{
	return nullptr;
}

void CaptureTower::Update(double dt)
{
	Tower::Update(dt);

	particleGenerator.Update(dt);

}

void CaptureTower::LevelUp()
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

void CaptureTower::Fire(double dt)
{
	Enemy* enemy = SearchEnemy(GetEnemyInRange());
	if (enemy)
	{
		if (enemy->b_isActive)
		{
			particleGenerator.pos = enemy->pos;
			particleGenerator.pos.z = 1;
			particleGenerator.maxVel = (this->pos - enemy->pos).Normalize() * p_speed;
			particleGenerator.minVel = (this->pos - enemy->pos).Normalize() * p_speed;
			particleGenerator.SpawnParticle();
		}
	}
}