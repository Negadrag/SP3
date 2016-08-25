#include "Minion.h"

Minion::Minion() :Enemy()
{
	this->meshID = GEO_BASIC;
	this->f_movSpeed = 3.f;
	this->f_health = 50;
	this->i_damage = 1;
	this->i_defence = 0;
	this->b_rotateUp = true;
	this->f_rotateSpeed = 90.f;
	this->f_clampRotate = 20.f;
	i_currency = 1;
}

Minion::Minion(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_SPEED;
	this->f_movSpeed = 3.f;
	this->f_health = 50;
	this->i_damage = 1;
	this->i_defence = 1;
	this->b_rotateUp = true;
	this->f_rotateSpeed = 90.f;
	this->f_clampRotate = 20.f;
	i_currency = 1;
}

Minion::~Minion()
{
}

void Minion::UpdateAnim(double dt)
{
	if (b_rotateUp == true)
	{
		this->rotation.y += f_rotateSpeed * dt;
		if (this->rotation.y >= f_clampRotate)
		{
			b_rotateUp = false;
			this->rotation.y = f_clampRotate;
		}
		
	}
	else
	{
		this->rotation.y -= f_rotateSpeed * dt;
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