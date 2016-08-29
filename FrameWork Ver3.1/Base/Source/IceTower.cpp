#include "IceTower.h"
#include "SplashTarget.h"
#include "IceProjectile.h"

int IceTower::cost = 45;
int IceTower::ecost = 100;
Tower::ESSENCE_TYPE IceTower::type = Tower::ESSENCE_TYPE::E_ICE;

IceTower::IceTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(5);
	SetRange(3);
	SetSpdRate(0.75f);
	this->p_speed = 5.f;
	this->f_SlowAmount = 20.f;
	this->f_SlowDura = 5.f;
	this->towerUpgradeCost = cost;
	this->essenceUpgradeCost = 100;
	this->essence = type;
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

IceProjectile* IceTower::GetProjectile()
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
			Music::GetInstance()->PlayMusic(3, false, 0.15f);
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

void IceTower::Fire(double dt)
{
	Enemy* enemy = SearchEnemy(GetEnemyInRange());
	if (enemy == nullptr || enemy->b_isActive == false)
	{
		f_rotationToBe = 270.f;
		return;
	}
	if (b_rotateWhenFire == true)
	{
		Vector3 view = enemy->pos - this->pos;
		f_rotationToBe = Math::RadianToDegree(atan2(view.y, view.x)); // the rotation that we want it to be at;

		f_rotationToBe = round(f_rotationToBe);


	}
	IceProjectile* projectile = GetProjectile();
	projectile->meshID = this->projectile_meshID;
	projectile->pos = this->pos + heightOffset;
	projectile->scale.Set(0.5f, 0.5f, 0.5f);
	projectile->p_speed = this->p_speed;
	projectile->enemy = enemy;
	projectile->enemyLastPos = enemy->pos;
	projectile->vel = (enemy->pos - projectile->pos).Normalize() * p_speed;
	projectile->i_damage = this->atkDamage;

	//projectileList.push_back(projectile);
	//enemy->i_health -= 1;
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
		this->f_SlowAmount += 10.f;
		return true;
	}
	return false;
}