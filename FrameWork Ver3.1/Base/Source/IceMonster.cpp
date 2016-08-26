#include "IceMonster.h"
#include "PlayerInfo.h"

IceMonster::IceMonster() :Enemy()
{
	this->meshID = GEO_ICE;
	this->f_movSpeed = 3.f;
	this->f_maxHealth = 50.f;
	this->f_health = f_maxHealth;
	this->i_damage = 1;
	this->i_defence = 15;
	this->f_rotateSpeed = 90.f;
	this->i_currency = 1;
	this->f_floatSpeed = 2.f;
	this->b_floatUp = true;
	this->f_translateTimer = 0.f;
}

IceMonster::IceMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_ICE;
	this->f_movSpeed = 3.f;
	this->f_maxHealth = 50.f;
	this->f_health = f_maxHealth;

	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	i_currency = 1;
	this->f_floatSpeed = 3.f;
	this->b_floatUp = true;
	this->f_translateTimer = 0.f;
}

IceMonster::~IceMonster()
{
}

void IceMonster::MoveTo(Vector2 dest, double dt)
{
	Vector3 view = Vector3(dest.x, dest.y, 0) - Vector3(pos.x, pos.y, 0);
	if (view.Length() == Math::EPSILON)
	{
		return;
	}
	view.Normalize();

	view = view * f_movSpeed *((float)(100 - f_slow) / 100.f) * dt;
	//view = view * f_movSpeed *dt;
	this->pos.x += view.x;
	this->pos.y += view.y;
}

void IceMonster::UpdateAnim(double dt)
{
	this->rotation.z += f_rotateSpeed * ((100.f - f_slow) / 100.f) * dt;
	if (this->rotation.z > 360.f)
	{
		this->rotation.z -= 360.f;
	}
	/*if (b_floatUp == true)
	{
		this->pos.z += f_floatSpeed * ((100.f - f_slow) / 100.f) * dt;
		if (this->pos.z > 1.f)
		{
			this->pos.z = 1.f;
			this->b_floatUp = false;
		}
	}
	else
	{

		this->pos.z -= f_floatSpeed * ((100.f - f_slow) / 100.f) * dt;
		if (this->pos.z < 0.5f)
		{
			this->pos.z = 0.5f;
			this->b_floatUp = true;
		}
	}*/
	
	this->f_translateTimer += f_floatSpeed * dt;

	this->pos.z = (sin(f_translateTimer)/3.f) + 1.1f;
}

void IceMonster::GiveEssence()
{
	//Enemy::GiveCurrency();
	player->i_essenceIce += 1;
}