#include "Enemy.h"

Enemy::Enemy()
{
	f_movSpeed = 5.f;
	i_health = 10;
	i_defence = 0;
	i_damage = 0;
	i_slow = 0;
	this->rotation.Set(0, 0, 0);
	this->scale.Set(1, 1, 1);
	this->pos.Set(0, 0, 0);
}

Enemy::Enemy(Vector3 pos, Node* root)
{
	f_movSpeed = 5.f;
	i_health = 10;
	i_defence = 0;
	i_damage = 0;
	i_slow = 0;
	this->rotation.Set(0, 0, 0);
	this->scale.Set(1, 1, 1);
	this->pos = pos;
	this->nxtTile = root;
}

Enemy::~Enemy()
{
}

void Enemy::MoveTo(Vector2 dest, double dt)
{
	float rotationSpeed = 360.f;
	Vector3 view = Vector3(dest.x,dest.y,0)- Vector3(pos.x, pos.y,0) ;
	if (view.Length() == Math::EPSILON)
	{
		return;
	}
	view.Normalize();
	//float rotationZToBe = Math::RadianToDegree(atan2(view.y,view.x)); // the rotation that we want it to be at;
	//
	//rotationZToBe = round(rotationZToBe);
	//
	//if (this->rotation.z != rotationZToBe)// to rotate the model if the enemy is turning
	//{
	//	if (rotationZToBe < 0) // making sure rotationZToBe is withiin 0 and 360
	//	{
	//		rotationZToBe += 360;
	//	}
	//	else if (rotationZToBe == -0)
	//	{
	//		rotationZToBe = 0;
	//	}
	//	
	//	if (abs(rotationZToBe - rotation.z) <180)
	//	{
	//		if (rotationZToBe > rotation.z)
	//		{

	//			rotation.z += rotationSpeed * dt;
	//			if (rotationZToBe < rotation.z)
	//			{
	//				rotation.z = rotationZToBe;
	//			}


	//		}
	//		else if (rotationZToBe < rotation.z)
	//		{
	//			rotation.z -= rotationSpeed * dt;
	//			if (rotationZToBe > rotation.z)
	//			{
	//				rotation.z = rotationZToBe;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (rotationZToBe > rotation.z)
	//		{

	//			rotation.z -= rotationSpeed * dt;
	//			if (rotation.z < 0.f) // wind around if negative numbers
	//			{
	//				rotation.z += 360;
	//			}
	//			else if (rotation.z == -0.f) // negate negative 0;
	//			{
	//				rotation.z = 0;
	//			}

	//		}
	//		else if (rotationZToBe < rotation.z)
	//		{
	//			rotation.z += rotationSpeed * dt;
	//			if (rotation.z >360.f)
	//			{
	//				rotation.z -= 360;
	//			}
	//		}
	//	}
	//}
	//view = view * f_movSpeed *((float)(100 - i_slow) / 100.f) * dt;
	view = view * f_movSpeed *dt;
	this->pos.x += view.x ;
	this->pos.y += view.y;
}

void Enemy::Update(double dt)
{
	//std::cout << "Updating" << std::endl;
	if (nxtTile != nullptr)
	{
		if ((Vector3(nxtTile->coords.x, nxtTile->coords.y, 0) - Vector3(this->pos.x, this->pos.y, 0)).LengthSquared() < 0.1f*0.1f)
		{
			nxtTile = nxtTile->next;
			if (nxtTile == nullptr)
			{
				return;
			}
		}
	
		MoveTo(nxtTile->coords, dt);
		UpdateAnim(dt);
	}
	else
	{
		this->b_isActive = false;
	}
}

void Enemy::UpdateAnim(double dt)
{
}

void Enemy::ReceiveDamage(int damage)
{
	int dmg = damage * ((100.f - (float)i_defence) / 100.f);
	this->i_health -= dmg;
	if (i_health <= 0)
	{
		this->b_isActive = false;
	}
}