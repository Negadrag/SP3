#include "SplashTarget.h"

SplashTarget::SplashTarget(GEOMETRY_TYPE ID) : Projectile(ID)
{
	f_range = 2.f;
	f_particleSpawnTimer = 0.f;
}

SplashTarget::~SplashTarget()
{
}

void SplashTarget::Update(double dt)
{
	Vector3 d;
	//if (this->b_isActive)
	//{

	if (enemy->b_isActive == false)
	{
		//iceparticle->ClearParticles();
		//this->iceparticle.isActive = false;
		this->b_isActive = false;
		return;
	}

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
		if (meshID == GEO_CANNON)
		{

				f_particleSpawnTimer = 0.f;

				iceparticle->pos = this->pos;
				iceparticle->pos.z = 2.f;
				iceparticle->SpawnParticle();
		}
		//this->iceparticle.isActive = false;
		this->b_isActive = false;
		if (enemyVec != nullptr)
		{
			for (vector<Enemy*>::iterator it = (*enemyVec).begin(); it != (*enemyVec).end(); ++it)
			{
				if ((*it)->b_isActive == true)
				{
					if (((*it)->pos - this->pos).LengthSquared() < f_range*f_range)

					(*it)->ReceiveDamage(i_damage/2);
				}
			}
		}
		

		enemy->ReceiveDamage(i_damage/2);
		//std::cout << d.LengthSquared() << std::endl;
	}
	
	//}

}