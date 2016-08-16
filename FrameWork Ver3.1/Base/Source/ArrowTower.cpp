#include "ArrowTower.h"

ArrowTower::ArrowTower()
:Tower()
{
	this->meshID = GEO_ARROWTOWER;
	SetAtkDmg(4);
	SetRange(10);
	SetSpdRate(2.f);
	this->p_speed = 10.f;
	this->projectile_meshID = GEO_ARROW;
	this->heightOffset.Set(0, 0, 2);
}

ArrowTower::~ArrowTower()
{

}

void ArrowTower::Update(double dt)
{
	Tower::Update(dt);

}