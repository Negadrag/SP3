#include "IceProjectile.h"

IceProjectile::IceProjectile(GEOMETRY_TYPE ID) : SplashTarget(ID)
{
	this->meshID = GEO_ICESHOT;
	this->f_slowDuration = 30.f;
	this->f_range = 2.f;
}

IceProjectile::~IceProjectile()
{
}

void IceProjectile::Update(double dt)
{
	Vector3 d;
	//if (this->b_isActive)
	//{

	if (meshID == GEO_ICESHOT)
	{
		Mtx44 rotate;
		rotate.SetToRotation(45.f, 0, 0, 1);

		iceparticle->minVel = rotate * -vel.Normalized();
		rotate.SetToIdentity();
		rotate.SetToRotation(-45.f, 0, 0, 1);
		iceparticle->maxVel = rotate * -vel.Normalized();
		iceparticle->pos = this->pos;
		iceparticle->SpawnParticle();
	}

	d = enemyLastPos - this->pos;
	//this->vel = (this->vel + d *( p_speed / 4.f)).Normalize() * p_speed;
	if (d.IsZero())
	{
		return;
	}
	this->vel = d.Normalized() * p_speed;
	rotation.z = Math::RadianToDegree(atan2(this->vel.y, this->vel.x));
	this->pos += vel * dt;
	float distanceToCheck = 0.04f * p_speed;
	if (d.LengthSquared() <= distanceToCheck * distanceToCheck)
	{

		//this->iceparticle.isActive = false;
		this->b_isActive = false;
		if (enemyVec != nullptr)
		{
			for (vector<Enemy*>::iterator it = (*enemyVec).begin(); it != (*enemyVec).end(); ++it)
			{
				if ((*it)->b_isActive == true)
				{
					if (((*it)->pos - this->pos).LengthSquared() < f_range*f_range)
					{

						(*it)->ReceiveSlowStatus(f_slowAmount, f_slowDuration);

						(*it)->ReceiveDamage(i_damage);
					}
				}
			}
		
		}

	}

}

