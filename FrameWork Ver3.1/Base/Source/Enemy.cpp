#include "Enemy.h"

Enemy::Enemy()
{
	i_movSpeed = 1;
	this->rotation.Set(90, 0, 0);
	this->scale.Set(1, 1, 1);
}

Enemy::Enemy(Vector3 pos, Node* root)
{
	i_movSpeed = 1;
	this->rotation.Set(90, 0, 0);
	this->scale.Set(1, 1, 1);
}

Enemy::~Enemy()
{
}

void Enemy::MoveTo(Vector2 dest, double dt)
{
	float rotationSpeed = 90.f;
	Vector2 view = dest - Vector2(pos.x, pos.y);
	if (view.Length() == 0)
	{
		view = Vector2(1, 0);
	}
	else
	{
		view = view.Normalized();
	}
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
	this->pos.x += view.x * i_movSpeed *dt;
	this->pos.y += view.y * i_movSpeed *dt;

}

void Enemy::Update(double dt)
{
	if (nxtTile != nullptr)
	{
		moveTo(nxtTile->coords, dt);
		if ((nxtTile->coords - Vector2(pos.x, pos.y)).Length() < 0.2f)
		{
			nxtTile = nxtTile->next;
		}
	}
	else
	{
		this->b_isActive = false;
	}
}