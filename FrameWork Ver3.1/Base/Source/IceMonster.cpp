#include "IceMonster.h"


IceMonster::IceMonster() :Enemy()
{
	this->meshID = GEO_ICE;
	this->f_movSpeed = 3.f;
	this->f_health = 100;
	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	this->i_currency = 1;
}

IceMonster::IceMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_ICE;
	this->f_movSpeed = 3.f;
	this->f_health = 50;

	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	i_currency = 1;
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
	this->rotation.z += f_rotateSpeed*dt;
	if (this->rotation.z > 360.f)
	{
		this->rotation.z -= 360.f;
	}
}