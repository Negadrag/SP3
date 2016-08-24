#include "CaptureTower.h"
#include "SingleTarget.h"

int CaptureTower::cost = 5;

CaptureTower::CaptureTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(0);
	SetRange(5);
	SetSpdRate(10.f);
	this->p_speed = 15.f;

	this->meshID = GEO_CAPTUREBASE;
	this->heightOffset.Set(0, 0, 1);
	this->strategy = FIRST_ENEMY;
	s_name = "Capture Tower";
	this->b_rotateWhenFire = false;

	particleGenerator.SetType(GEO_ESSENCE);
	particleGenerator.SetFrequency(1);
	particleGenerator.SetCap(1000);
	particleGenerator.i_spawnAmount = 10;
	particleGenerator.f_lifeTime = 2.f;
	particleGenerator.minVel.Set(0, 0, 0);
	particleGenerator.maxVel.Set(0, 0, 0);
	particleGenerator.scale.Set(0.35f, 0.35f, 0.35f);
	//particleGenerator.i_particleCount = 0;
	particleGenerator.f_maxDist = 5.f;
	particleGenerator.isActive = false;

	child.meshID = GEO_CAPTURERING1;
	child2.SetParent(this);
	child2.meshID = GEO_CAPTURERING1;
	child2.scale.Set(0.7f, 0.7f, 0.7f);
	child2.pos.Set(0, 0, 2.5f);
	child2.b_isActive = true;
	child3.SetParent(this);
	child3.meshID = GEO_CAPTUREORB;
	child3.scale.Set(0.7f, 0.7f, 0.7f);
	child3.pos.Set(0, 0, 2.5f);
	child3.b_isActive = true;
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

	if (child2.meshID == GEO_CAPTURERING1)
	{
		child2.pos.z = 2.5f;
		child2.rotation.y += 30.f * dt;
		child2.rotation.z += 30.f * dt;
	}

	if (child.meshID == GEO_CAPTURERING1)
	{
		child.pos.z = 2.5f;
		child.rotation.y += 30.f * dt;
	}

	if (child3.meshID == GEO_CAPTUREORB)
	{
		child3.pos.z = 2.5f;
		child3.rotation.z += 30.f * dt;
	}
}

void CaptureTower::LevelUp()
{
	if (this->i_level <= 2)
	{
		this->i_level++;
		this->atkRange += 1;
		if (atkRange > 7)
		{
			atkRange = 7;
		}
		if (this->i_level >= 2)
		{
			i_level = 2;
		}
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