#include "Particles.h"

Particles::Particles(GEOMETRY_TYPE ID) :
vel(0, 0, 0)
, rotation(0)
, rotationSpeed(0)
{
	meshID = ID;
	b_3DbillBoard = true;
	b_lightEnabled = false;
	b_shadows = false;
	b_isActive = false;
	lifeTime = 0.f;
	f_distTravelled = 0.f;
	f_maxDist = 0.f;
}

Particles::~Particles()
{
}

void Particles::Update(double dt, int& particle_count)
{
	Vector3 m_gravity;
	if (this->b_isActive)
	{
		if (meshID == GEO_WATER)//change to water later
		{
			this->vel += m_gravity * (float)dt;
			this->pos += this->vel * (float)dt * 10.f;
			if (this->pos.y < 0)//replace with readfromheightmap
			{
				this->b_isActive = false;
				--particle_count;
			}
		}
		if (this->meshID == GEO_SMOKEPARTICLES)
		{
			lifeTime -= dt;
			this->pos += vel;
			if (this->lifeTime<0.f)
			{
				this->b_isActive = false;
				//--particle_count;
			}
		}

		if (this->meshID == GEO_PARTICLE_WHITE)
		{
			lifeTime -= dt;
			this->pos += vel * dt;
			this->scale -= Vector3(dt * 5, dt * 5, 0);
			if (scale.x <= 0.2f || scale.y <= 0.2f)
			{
				this->b_isActive = false;
				--particle_count;
			}
			this->f_distTravelled += vel.Length() * dt;
			if (this->lifeTime<0.f || f_distTravelled >= f_maxDist)
			{
				this->b_isActive = false;
				--particle_count;
			}
		}
		if (this->meshID == GEO_PARTICLE_BLUE)
		{
			lifeTime -= dt;
			this->pos += vel * dt;
			this->f_distTravelled += vel.Length() * dt;
			if (this->lifeTime<0.f || f_distTravelled >= f_maxDist)
			{
				this->b_isActive = false;
				--particle_count;
			}
		}
		if (this->meshID == GEO_PARTICLE_RED)
		{
			lifeTime -= dt;
			this->pos += vel * dt;
			this->scale -= Vector3(dt/2 , dt/2 , 0);
			if (scale.x <= 0.2f || scale.y <= 0.2f)
			{
				this->b_isActive = false;
				--particle_count;
			}
			this->f_distTravelled += vel.Length() * dt;
			if (this->lifeTime<0.f || f_distTravelled >= f_maxDist)
			{
				this->b_isActive = false;
				--particle_count;
			}
		}
		if (this->meshID == GEO_PARTICLE_YELLOW)
		{
			lifeTime -= dt;
			this->pos += vel * dt;
			this->f_distTravelled += vel.Length() * dt;
			if (this->lifeTime<0.f || f_distTravelled >= f_maxDist)
			{
				this->b_isActive = false;
				--particle_count;
			}
		}
		if (this->meshID == GEO_PARTICLE_GREEN)
		{
			lifeTime -= dt;
			this->pos += vel * dt;
			this->f_distTravelled += vel.Length() * dt;
			if (this->lifeTime<0.f || f_distTravelled >= f_maxDist)
			{
				this->b_isActive = false;
				--particle_count;
			}
		}
		if (this->meshID == GEO_ICEPARTICLE || this->meshID == GEO_CANNONBLAST || this->meshID == GEO_ESSENCE)
		{
			lifeTime -= dt;
			this->scale -= Vector3(0.2f, 0.2f, 0.2f)*dt;
			if (scale.IsZero())
			{
				scale.Set(0.03f, 0.03f, 0.03f);
			}
			if (scale.x<0.02f||scale.y<0.02f||scale.z<0.02f)
			{
				scale.Set(0.03f, 0.03f, 0.03f);
			}
			this->pos += vel * dt;
			this->f_distTravelled += vel.Length() * dt;
			if (this->lifeTime<0.f || f_distTravelled >= f_maxDist)
			{
				this->b_isActive = false;
				--particle_count;
			}
		}

	}

}