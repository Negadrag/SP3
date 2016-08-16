#include "Enemy.h"

Enemy::Enemy()
{
	f_movSpeed = 6.f;
	this->rotation.Set(90, 0, 0);
	this->scale.Set(1, 1, 1);
	this->pos.Set(0, 0, 0);
}

Enemy::Enemy(Vector3 pos, Node* root)
{
	f_movSpeed = 6.f;
	this->rotation.Set(90, 0, 0);
	this->scale.Set(1, 1, 1);
	this->pos.Set(0, 0, 0);
}

Enemy::~Enemy()
{
}

void Enemy::MoveTo(Vector3 dest, double dt)
{
	float rotationSpeed = 90.f;
	Vector3 view = Vector3(dest.x, dest.y, 0)- Vector3(pos.x, pos.y, 0) ;
	if (view.Length() == Math::EPSILON)
	{
		return;
	}
	view.Normalize();
	float rotationZToBe = Math::RadianToDegree(atan2(view.y,view.x)); // the rotation that we want it to be at;

	if (this->rotation.z != rotationZToBe)// to rotate the model if the enemy is turning
	{
		if (rotationZToBe > rotation.z )
		{
			rotation.z += rotationSpeed * dt;
			if (rotationZToBe < rotation.z)
			{
				rotation.z = rotationZToBe;
			}
		}
		else
		{
			rotation.z -= rotationSpeed * dt;
			if (rotationZToBe > rotation.z)
			{
				rotation.z = rotationZToBe;
			}
		}
	}
	this->pos.x += view.x * f_movSpeed *dt;
	this->pos.y += view.y * f_movSpeed *dt;

}

void Enemy::Update(double dt)
{
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
		MoveTo(Vector3(nxtTile->coords.x,nxtTile->coords.y), dt);
		
	}
	else
	{
		this->b_isActive = false;
	}
}