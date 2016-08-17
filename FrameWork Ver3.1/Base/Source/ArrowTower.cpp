#include "ArrowTower.h"

ArrowTower::ArrowTower()
:Tower()
{
	this->meshID = GEO_ARROWTOWER;
	SetAtkDmg(10);
	SetRange(5);
	SetSpdRate(4.f);
	this->p_speed = 10.f;
	this->projectile_meshID = GEO_ARROW;
	this->heightOffset.Set(0, 0, 2);
	this->strategy = FIRST_ENEMY;
}

ArrowTower::~ArrowTower()
{

}

void ArrowTower::Update(double dt)
{
	Tower::Update(dt);

}