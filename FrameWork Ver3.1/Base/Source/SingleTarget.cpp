#include "SingleTarget.h"

SingleTarget::SingleTarget(GEOMETRY_TYPE ID) : Projectile(ID)
{
	
}

SingleTarget::~SingleTarget()
{
}

void SingleTarget::Update(double dt)
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
	if (enemy->b_isActive == false)
	{
		this->b_isActive = false;
	}
	float distanceToCheck = 0.04f * p_speed;
	if (d.LengthSquared() <= distanceToCheck * distanceToCheck)
	{
		this->b_isActive = false;
		enemy->ReceiveDamage(i_damage);
		//std::cout << d.LengthSquared() << std::endl;
	}


	//}

}