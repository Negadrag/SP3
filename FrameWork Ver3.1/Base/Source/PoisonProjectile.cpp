#include "PoisonProjectile.h"

PoisonProjectile::PoisonProjectile(GEOMETRY_TYPE ID) : SingleTarget(ID)
{
	this->meshID = GEO_POISONARROW;
	this->f_poisondmg = 5.f;
	this->f_poisondura = 30.f;
}

PoisonProjectile::~PoisonProjectile()
{
}

void PoisonProjectile::Update(double dt)
{

}