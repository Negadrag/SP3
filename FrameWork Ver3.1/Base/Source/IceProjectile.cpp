#include "IceProjectile.h"

IceProjectile::IceProjectile(GEOMETRY_TYPE ID) : SplashTarget(ID)
{
	this->meshID = GEO_ICESHOT;
	this->f_frostdura = 30.f;
}

IceProjectile::~IceProjectile()
{
}

void IceProjectile::Update(double dt)
{

}