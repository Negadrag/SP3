#include "Particles.h"

Particles::Particles(GEOMETRY_TYPE ID):
 vel(0,0,0)
, rotation(0)
, rotationSpeed(0)
{
	meshID = ID;
	b_3DbillBoard = true;
	b_lightEnabled = false;
	b_shadows = false;
	b_isActive = false;
	lifeTime = 0.f;
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
				--particle_count;
			}
		}
	}
	
}