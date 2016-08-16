#include "CannonTower.h"

CannonTower::CannonTower()
:Tower()
{
	this->meshID = GEO_CANNONTOWER;
	SetAtkDmg(10);
	SetRange(10);
	SetSpdRate(1.f);
	this->p_speed = 5.f;
	this->projectile_meshID = GEO_CANNON;
	this->heightOffset.Set(0, 0, 2);
}

CannonTower::~CannonTower()
{

}

void CannonTower::Update(double dt)
{
	Tower::Update(dt);

}