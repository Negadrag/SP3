#include "Minion.h"
#include "PlayerInfo.h"

Minion::Minion() :Enemy()
{
	this->meshID = GEO_BASIC;
	this->f_movSpeed = 3.f;
	this->f_maxHealth = 50.f;
	this->f_health = f_maxHealth;
	this->i_damage = 1;
	this->i_defence = 10;
	this->b_rotateUp = true;
	this->f_rotateSpeed = 90.f;
	this->f_clampRotate = 20.f;
	i_currency = 1;
}

Minion::Minion(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_BASIC;
	this->f_movSpeed = 3.f;
	this->f_maxHealth = 50.f;
	this->f_health = f_maxHealth;
	this->i_damage = 1;
	this->i_defence = 10;
	this->b_rotateUp = true;
	this->f_clampRotate = 20.f;
	this->i_currency = 1;
	this->f_rotateSpeed = 90.f;
}

Minion::~Minion()
{
}

void Minion::UpdateAnim(double dt)
{
	if (b_rotateUp == true)
	{
		this->rotation.y += f_rotateSpeed * ((100.f-f_slow)/100.f) * dt;
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

	//this->rotation.z += f_rotateSpeed*dt;
	//if (this->rotation.z > 360.f)
	//{
	//	this->rotation.z -= 360.f;
	//}
}

void Minion::GiveEssence()
{
	//Enemy::GiveCurrency();
	player->i_essenceBasic += 1;
}

void Minion::ReceivePoisonStatus(float poisonDPS, float slowAmount, float duration)
{
	this->ReceiveSlowStatus(slowAmount, duration);
}

void Minion::UpdateMesh()
{
	if (f_slowTimer > 0)
	{
		this->meshID = GEO_BASIC_FROST;
	}
	else
	{
		this->meshID = GEO_BASIC;
	}
}
