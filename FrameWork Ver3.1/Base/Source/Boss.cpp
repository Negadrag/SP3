#include "Boss.h"
#include "PlayerInfo.h"

Boss::Boss() :Enemy()
{
	this->zVel = 0.f;
	this->b_onGround = true;
}


Boss::Boss(Vector3 pos, Node* root, vector<Tower*>* towerlist) :Enemy(pos, root)
{
	this->meshID = GEO_BOSS;
	this->f_movSpeed = 1.5f;
	this->f_maxHealth = 500.f;
	this->f_health = f_maxHealth;

	this->towerList = towerlist;
	this->i_damage = 1;
	this->i_defence = 50;
	this->i_currency = 50;
	this->scale.Set(1.5f,1.5f,1.5f);
}

Boss::~Boss()
{
}

void Boss::Update(double dt)
{
	UpdateMesh();

	if (nxtTile != nullptr)
	{
		if (b_onGround == true)
		{

			if ((Vector3(nxtTile->coords.x, nxtTile->coords.y, 0) - Vector3(this->pos.x, this->pos.y, 0)).LengthSquared() < 0.1f*0.1f)
			{
				nxtTile = nxtTile->next;
				if (nxtTile)
				{
					targetPos = nxtTile->coords;
				}
				
			}

			float rotationSpeed = 360.f;
			Vector3 view = Vector3(targetPos.x, targetPos.y, 0) - Vector3(pos.x, pos.y, 0);
			if (view.Length() == Math::EPSILON)
			{
				return;
			}
			view.Normalize();
			float rotationZToBe = Math::RadianToDegree(atan2(view.y, view.x)); // the rotation that we want it to be at;

			rotationZToBe = round(rotationZToBe);
			bool finishedRotating = true;
			if (this->rotation.z != rotationZToBe)// to rotate the model if the enemy is turning
			{
				if (rotationZToBe < 0) // making sure rotationZToBe is withiin 0 and 360
				{
					rotationZToBe += 360;
				}
				else if (rotationZToBe == -0)
				{
					rotationZToBe = 0;
				}

				if (abs(rotationZToBe - rotation.z) <180)
				{
					if (rotationZToBe > rotation.z)
					{

						rotation.z += rotationSpeed * dt;
						if (rotationZToBe < rotation.z)
						{
							rotation.z = rotationZToBe;
						}

					}
					else if (rotationZToBe < rotation.z)
					{
						rotation.z -= rotationSpeed * dt;
						if (rotationZToBe > rotation.z)
						{
							rotation.z = rotationZToBe;
						}
						
					}
				}
				else
				{
					if (rotationZToBe > rotation.z)
					{

						rotation.z -= rotationSpeed * dt;
						if (rotation.z < 0.f) // wind around if negative numbers
						{
							rotation.z += 360;
						}
						else if (rotation.z == -0.f) // negate negative 0;
						{
							rotation.z = 0;
						}
						

					}
					else if (rotationZToBe < rotation.z)
					{
						rotation.z += rotationSpeed * dt;
						if (rotation.z >360.f)
						{
							rotation.z -= 360;
						}
						
					}
				}
				if (rotation.z != rotationZToBe)
				{
					finishedRotating = false;
				}
			}
			if (finishedRotating == true)
			{
				b_onGround = false;
				zVel = 4.f;
			}
		}
		else
		{
			MoveTo(targetPos, dt);
			UpdateAnim(dt);
		}
		if (f_poisonTimer > 0.f)
		{
			ReceivePoisonDamage(f_poisonDps*dt);
			f_poisonTimer -= dt;
			if (f_poisonTimer <= 0.f)
			{
				f_poisonTimer = 0.f;
			}
		}
		if (f_slowTimer > 0.f)
		{
			f_slowTimer -= dt;
			if (f_slowTimer <= 0.f)
			{
				f_slow = 0.f;
				f_slowTimer = 0.f;
			}
		}
		if (this->f_showHealthTimer > 0.f)
		{
			f_showHealthTimer -= dt;
		}
		else if (this->f_showHealthTimer < 0.f)
		{
			f_showHealthTimer = 0.f;
			hp.b_Render = false;
		}
	}
	if (nxtTile)
	{
		if (nxtTile->next == nullptr && (Vector3(nxtTile->coords.x, nxtTile->coords.y, 0) - Vector3(this->pos.x, this->pos.y, 0)).LengthSquared() < 0.1f*0.1f)
		{
			this->b_isActive = false;
			this->hp.b_isActive = false;
			this->b_isActive = false;

			if (player)
			{
				player->i_health -= i_damage;
			}
		}
	}
}

void Boss::UpdateAnim(double dt)
{
	this->zVel -= 4 * dt;
	this->pos.z += this->zVel * dt;
	if (this->pos.z <= 0.f)
	{
		this->pos.z = 0.f;
		this->b_onGround = true;
	}
}

void Boss::GiveEssence()
{
}

void Boss::MoveTo(Vector2 dest, double dt)
{
	Vector3 view = Vector3(dest.x, dest.y, 0) - Vector3(pos.x, pos.y, 0);
	if (view.Length() == Math::EPSILON)
	{
		return;
	}
	view.Normalize();
	if (this->f_showHealthTimer > 0.f)
	{
		hp.pos.Set(0, 0, 1);
		hp.b_Render = true;
		hp.pos = this->pos + Vector3(0, 0, 1);
		hp.rotation.z = this->rotation.z;
		hp.scale = Vector3(0.2f, f_health / f_maxHealth, 0.1f);
	}
	view = view * f_movSpeed *((float)(100.f - f_slow) / 100.f) * dt;
	//view = view * f_movSpeed *dt;
	this->pos.x += view.x;
	this->pos.y += view.y;
}

void  Boss::UpdateMesh()
{
	if (f_poisonTimer > 0)
	{
		this->meshID = GEO_BOSS_POISON;
	}
	else if (f_slowTimer > 0)
	{
		this->meshID = GEO_BOSS_FROST;
	}
	else
	{
		this->meshID = GEO_BOSS;
	}
}
