#include "Minion.h"

Minion::Minion() :Enemy()
{
	this->meshID = GEO_TANKY;
	this->f_movSpeed = 3.f;
	this->i_health = 100;
	this->i_damage = 1;
	this->i_defence = 0;
}

Minion::Minion(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_BASIC;
	this->f_movSpeed = 3.f;
	this->i_health = 500;
	this->i_damage = 1;
	this->i_defence = 0;
}

Minion::~Minion()
{
}

