#include "IceMonster.h"
#include "PlayerInfo.h"

IceMonster::IceMonster() :Enemy()
{
	this->meshID = GEO_ICE;
	this->f_movSpeed = 3.f;
	this->f_maxHealth = 50.f;
	this->f_health = f_maxHealth;
	this->i_damage = 1;
	this->i_defence = 15;
	this->f_rotateSpeed = 90.f;
	this->i_currency = 2;
	this->f_floatSpeed = 2.f;
	this->b_floatUp = true;
	this->f_translateTimer = 0.f;
}

IceMonster::IceMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_ICE;
	this->f_movSpeed = 3.f;
	this->f_maxHealth = 50.f;
	this->f_health = f_maxHealth;

	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	this->i_currency = 2;
	this->f_floatSpeed = 3.f;
	this->b_floatUp = true;
	this->f_translateTimer = 0.f;
	this->f_rotationZToBe = 0.f;
}

IceMonster::~IceMonster()
{
}

void IceMonster::MoveTo(Vector2 dest, double dt)
{
	Vector3 view = Vector3(dest.x, dest.y, 0) - Vector3(pos.x, pos.y, 0);
	if (view.Length() == Math::EPSILON)
	{
		return;
	}
	view.Normalize();
	f_rotationZToBe = Math::RadianToDegree(atan2(view.y, view.x)); // the rotation that we want it to be at;
	float rotationSpeed = 360.f;
	f_rotationZToBe = round(f_rotationZToBe);
	if (this->f_showHealthTimer > 0.f)
	{
		if (this->hp.rotation.z != f_rotationZToBe)// to rotate the model if the enemy is turning
		{
			if (f_rotationZToBe < 0) // making sure f_rotationZToBe is withiin 0 and 360
			{
				f_rotationZToBe += 360;
			}
			else if (f_rotationZToBe == -0)
			{
				f_rotationZToBe = 0;
			}

			if (abs(f_rotationZToBe - hp.rotation.z) <180)
			{
				if (f_rotationZToBe > hp.rotation.z)
				{

					hp.rotation.z += rotationSpeed * dt;
					if (f_rotationZToBe < hp.rotation.z)
					{
						hp.rotation.z = f_rotationZToBe;
					}


				}
				else if (f_rotationZToBe < hp.rotation.z)
				{
					hp.rotation.z -= rotationSpeed * dt;
					if (f_rotationZToBe > hp.rotation.z)
					{
						hp.rotation.z = f_rotationZToBe;
					}
				}
			}
			else
			{
				if (f_rotationZToBe > hp.rotation.z)
				{

					hp.rotation.z -= rotationSpeed * dt;
					if (hp.rotation.z < 0.f) // wind around if negative numbers
					{
						hp.rotation.z += 360;
					}
					else if (hp.rotation.z == -0.f) // negate negative 0;
					{
						hp.rotation.z = 0;
					}

				}
				else if (f_rotationZToBe < hp.rotation.z)
				{
					hp.rotation.z += rotationSpeed * dt;
					if (hp.rotation.z >360.f)
					{
						hp.rotation.z -= 360;
					}
				}
			}
		
		}
		hp.pos.Set(0, 0, 1);
		hp.b_Render = true;
		hp.pos = this->pos + Vector3(0, 0, 1);
		//hp.rotation.z = f_rotationZToBe;
		hp.scale = Vector3(0.2f, f_health / f_maxHealth, 0.1f);
	}
	view = view * f_movSpeed *((float)(100 - f_slow) / 100.f) * dt;
	//view = view * f_movSpeed *dt;
	this->pos.x += view.x;
	this->pos.y += view.y;
}

void IceMonster::UpdateAnim(double dt)
{
	this->rotation.z += f_rotateSpeed * ((100.f - f_slow) / 100.f) * dt;
	if (this->rotation.z > 360.f)
	{
		this->rotation.z -= 360.f;
	}
	/*if (b_floatUp == true)
	{
		this->pos.z += f_floatSpeed * ((100.f - f_slow) / 100.f) * dt;
		if (this->pos.z > 1.f)
		{
			this->pos.z = 1.f;
			this->b_floatUp = false;
		}
	}
	else
	{

		this->pos.z -= f_floatSpeed * ((100.f - f_slow) / 100.f) * dt;
		if (this->pos.z < 0.5f)
		{
			this->pos.z = 0.5f;
			this->b_floatUp = true;
		}
	}*/
	
	this->f_translateTimer += f_floatSpeed * dt;

	this->pos.z = (sin(f_translateTimer)/3.f) + 1.1f;
}

void IceMonster::GiveEssence()
{
	//Enemy::GiveCurrency();
	player->i_essenceIce += 1;
}

void IceMonster::ReceiveSlowStatus(float slowAmount, float slowDuration)
{
}

void  IceMonster::UpdateMesh()
{
	if (f_poisonTimer > 0)
	{
		this->meshID = GEO_ICE_POISON;
	}
	else
	{
		this->meshID = GEO_ICE;
	}
}

void IceMonster::ReceiveDamage(float damage)
{
	float dmg = damage * ((100.f - (float)i_defence) / 100.f);
	this->f_health -= dmg;
	if (f_health <= 0)
	{
		this->GiveCurrency();
		this->hp.b_isActive = false;
		this->b_isActive = false;
	}
	else
	{
		if (f_showHealthTimer <= 0.1f)
		{
			this->hp.rotation.z = f_rotationZToBe;
		}
		f_showHealthTimer = 1.5f;
	}
}

void IceMonster::ReceivePoisonDamage(float damage)
{

	this->f_health -= damage;
	if (f_health <= 0)
	{
		this->GiveCurrency();
		this->hp.b_isActive = false;
		this->b_isActive = false;
	}
	else
	{
		if (f_showHealthTimer < 0.1f)
		{
			this->hp.rotation.z = f_rotationZToBe;
		}
		f_showHealthTimer = 1.5f;
	}
}