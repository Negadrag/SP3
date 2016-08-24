#include "SpeedMonster.h"


SpeedMonster::SpeedMonster() :Enemy()
{
	this->meshID = GEO_SPEED;
	this->f_movSpeed = 3.f;
	this->f_health = 100;
	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	this->i_currency = 1;
}

SpeedMonster::SpeedMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_SPEED;
	this->f_movSpeed = 3.f;
	this->f_health = 50;

	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	i_currency = 1;
}

SpeedMonster::~SpeedMonster()
{
}

void SpeedMonster::UpdateAnim(double dt)
{
}