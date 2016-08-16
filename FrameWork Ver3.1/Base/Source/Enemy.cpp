#include "Enemy.h"

Enemy::Enemy()
{
	f_movSpeed = 3.f;
	this->rotation.Set(90, 0, 0);
	this->scale.Set(1, 1, 1);
}

Enemy::Enemy(Vector3 pos, Node* root)
{
	f_movSpeed = 3.f;
	this->rotation.Set(90, 0, 0);
	this->scale.Set(1, 1, 1);
}

Enemy::~Enemy()
{
}

void Enemy::MoveTo(Vector2 dest, double dt)
{
	float rotationSpeed = 360.f;
	Vector2 view = dest- Vector2(pos.x, pos.y) ;
	if (view.Length() == Math::EPSILON)
	{
		return;
	}
	view.Normalized();
	float rotationZToBe = Math::RadianToDegree(atan2(view.y,view.x)); // the rotation that we want it to be at;
	rotationZToBe = round(rotationZToBe);
	

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
		MoveTo(nxtTile->coords, dt);
		
	}
	else
	{
		this->b_isActive = false;
	}
}