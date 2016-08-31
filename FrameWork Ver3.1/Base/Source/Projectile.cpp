#include "Projectile.h"

Projectile::Projectile(GEOMETRY_TYPE ID) :
vel(0, 0, 0)
{
	this->meshID = ID;// ID;
	this->b_shadows = false;
}

Projectile::Projectile(Vector3 pos, Vector3 scale)
{
	this->pos = pos;
	this->scale = scale;
	this->b_shadows = false;
}

Projectile::~Projectile()
{
}

void Projectile::Update(double dt)
{
	Vector3 d;
	
	//if (this->b_isActive)
	//{
		if (enemy->b_isActive == false)
		{
			this->b_isActive = false;
			return;
		}

			d = enemy->pos - this->pos;
			//this->vel = (this->vel + d *( p_speed / 4.f)).Normalize() * p_speed;
			if (d.IsZero())
			{
				return;
			}
			this->vel = d.Normalized() * p_speed;
			rotation.z = Math::RadianToDegree(atan2(this->vel.y, this->vel.x));
			this->pos += vel * dt;
			
			float distanceToCheck = 0.06f * p_speed;
			if (d.LengthSquared() <= distanceToCheck * distanceToCheck)
			{
				this->b_isActive = false;
				enemy->ReceiveDamage(i_damage);
			
				//std::cout << d.LengthSquared() << std::endl;
			}

			
	//}

}