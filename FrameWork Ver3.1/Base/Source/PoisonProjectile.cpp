#include "PoisonProjectile.h"

PoisonProjectile::PoisonProjectile(GEOMETRY_TYPE ID) : SingleTarget(ID)
{
	this->meshID = GEO_POISONARROW;
	this->f_poisondmg = 5.f;
	this->f_poisondura = 5.f;
	this->f_slowAmount = 0.f;
}

PoisonProjectile::~PoisonProjectile()
{
}

void PoisonProjectile::Update(double dt)
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
		if (meshID == GEO_POISONARROW)
		{
			enemy->ReceivePoisonStatus(f_poisondmg, f_slowAmount,f_poisondura);
		}
		enemy->ReceiveDamage(i_damage);
		//std::cout << d.LengthSquared() << std::endl;
	}
}