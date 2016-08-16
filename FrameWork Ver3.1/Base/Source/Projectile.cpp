#include "Projectile.h"

Projectile::Projectile(GEOMETRY_TYPE ID) :
vel(0, 0, 0)
{
	this->meshID = ID;// ID;

}

Projectile::Projectile(Vector3 pos, Vector3 scale)
{
	this->pos = pos;
	this->scale = scale;

}

Projectile::~Projectile()
{
}

void Projectile::Update(double dt)
{
	Vector3 d;
	
	//if (this->b_isActive)
	//{

			d = enemy->pos - this->pos;
			this->vel = (this->vel + d * 2.25f).Normalize() * p_speed;
			rotation.z = Math::RadianToDegree(atan2(this->vel.y, this->vel.x));
			this->pos += vel * dt;
			if (enemy->b_isActive == false)
			{
				this->b_isActive = false;
			}
			if (d.LengthSquared() <= 0.5f * 0.5f)
			{
				this->b_isActive = false;
			}


	//}

}