#include "SplashTarget.h"

SplashTarget::SplashTarget(GEOMETRY_TYPE ID) : Projectile(ID)
{
	f_range = 2.f;

	iceparticle.SetType(GEO_ICEPARTICLE);
	iceparticle.SetFrequency(20);
	iceparticle.SetCap(1000);
	iceparticle.i_spawnAmount = 30;
	iceparticle.f_lifeTime = 0.3f;
	iceparticle.minVel.Set(0,0,0);
	iceparticle.maxVel.Set(0,0,0);
	iceparticle.scale.Set(0.07f,0.07f,0.07f);
	iceparticle.i_particleCount = 0;
	iceparticle.f_maxDist = 2.f;
	iceparticle.isActive = true;
	iceparticle.i_spawnAmount = 10;
}

SplashTarget::~SplashTarget()
{
}

void SplashTarget::Update(double dt)
{
	Vector3 d;
	if (meshID == GEO_CANNON)
	{
		Mtx44 rotate;
		rotate.SetToRotation(45.f, 0, 0, 1);

		iceparticle.minVel = rotate * -vel.Normalized();
		rotate.SetToIdentity();
		rotate.SetToRotation(-45.f, 0, 0, 1);
		iceparticle.maxVel = rotate * -vel.Normalized();
		iceparticle.pos = this->pos;
		iceparticle.Update(dt);
	}

	//if (this->b_isActive)
	//{
	if (enemy->b_isActive == false)
	{
		this->iceparticle.ClearParticles();
		//this->iceparticle.isActive = false;
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
		this->iceparticle.ClearParticles();
		//this->iceparticle.isActive = false;
		this->b_isActive = false;
		if (enemyVec)
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