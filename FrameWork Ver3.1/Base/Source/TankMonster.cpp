#include "TankMonster.h"
#include "PlayerInfo.h"

TankMonster::TankMonster() :Enemy()
{
}

TankMonster::TankMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_TANKY;
	this->f_movSpeed = 1.5f;
	this->f_maxHealth = 100.f;
	this->f_health = f_maxHealth;

	this->i_damage = 1;
	this->i_defence = 30;
	this->i_currency = 2;
	this->scale.Set(0.75f, 0.75f, 0.75f);
	this->b_rotateUp = true;
	this->f_clampRotate = 20.f;
	this->f_rotateSpeed = 50.f;
}

TankMonster::~TankMonster()
{
}

void TankMonster::UpdateAnim(double dt)
{
	if (b_rotateUp == true)
	{
		this->rotation.y += f_rotateSpeed * ((100.f - f_slow) / 100.f) * dt;
		if (this->rotation.y >= f_clampRotate)
		{
			b_rotateUp = false;
			this->rotation.y = f_clampRotate;
		}

	}
	else
	{
		this->rotation.y -= f_rotateSpeed * ((100.f - f_slow) / 100.f) * dt;
		if (this->rotation.y <= -f_clampRotate)
		{
			b_rotateUp = true;
			this->rotation.y = -f_clampRotate;
		}
	}
}

void TankMonster::GiveEssence()
{
	//Enemy::GiveCurrency();
	player->i_essenceTanky += 1;
}

void TankMonster::UpdateMesh()
{
	if (f_poisonTimer > 0)
	{
		this->meshID = GEO_TANKY_POISON;
	}
	else if (f_slowTimer > 0)
	{
		this->meshID = GEO_TANKY_FROST;
	}
	else
	{
		this->meshID = GEO_TANKY;
	}
}
