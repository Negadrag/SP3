#include "MortarTower.h"
#include "SplashTarget.h"
#include "FixedProjectile.h"

int MortarTower::cost = 200;
int MortarTower::ecost = 200;
Tower::ESSENCE_TYPE MortarTower::type = Tower::ESSENCE_TYPE::E_HEAVY;

MortarTower::MortarTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetAtkDmg(400);
	SetRange(10);
	SetSpdRate(0.2f);
	this->p_speed = 5.f;
	this->towerUpgradeCost = cost;
	this->essenceUpgradeCost = 100;
	this->essence = type;
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

FixedProjectile* MortarTower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		FixedProjectile* projectile = (FixedProjectile*)(*it);
		if (!(projectile->b_isActive))
		{
			projectile->b_isActive = true;
			projectile->meshID = projectile_meshID;
			projectile->enemyVec = this->enemyList;
			projectile->iceparticle = &particleGenerator;
			Music::GetInstance()->PlayMusic(4, false, 0.15f);
			return projectile;

		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{

		FixedProjectile* projectile = new FixedProjectile(projectile_meshID);
		projectile->b_isActive = false;
		projectile->iceparticle = &particleGenerator;
		projectileList.push_back(projectile);
	}

	return GetProjectile();
}

void MortarTower::Fire(double dt)
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
	FixedProjectile* projectile = GetProjectile();
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
		if (i_level == 2)
		{
			this->atkSpeed = 0.3f;
		}
		if (i_level == 3)
		{
			this->atkSpeed = 0.5f;
		}
		return true;
	}
	return false;
}

void MortarTower::UpdateMesh()
{
	if (b_isFrozen == true)
	{
		meshID = GEO_MORTARTOWER_FROST;
		child.meshID = GEO_MORTARCANNON_FROST;
	}
	else
	{
		meshID = GEO_MORTARBASE;
		child.meshID = GEO_MORTARCANNON;
	}
}