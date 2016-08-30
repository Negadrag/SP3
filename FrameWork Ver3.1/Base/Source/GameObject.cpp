#include "Music.h"
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	//scale(1, 1, 1),
	//active(false),
	mass(1.f),
	vel(Vector3(0,0,0))
{
	this->b_shadows = true;
}

GameObject::~GameObject()
{
}

bool GameObject::CheckCollision(GameObject* go2 , double dt)
{
	if (this->type != GameObject::GO_BALL && go2->type != GameObject::GO_BALL)
	{
		return false;
	}
	GameObject* go;
	GameObject* other;
	if (this->type != GameObject::GO_BALL)
	{
		go = go2;
		other = this;
	}
	else
	{
		go = this;
		other = go2;
	}
	switch (other->type)
	{
		case GameObject::GO_BALL:
		{
			float distanceSquared = ((go->pos + go->vel* dt) - (other->pos - other->vel * dt)).LengthSquared();
			float combineRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);

			Vector3 relativeVelocity = go->vel - other->vel;
			Vector3 relativeDisplacement = other->pos - go->pos;

			return distanceSquared < combineRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0;
			break;
		}
		case GameObject::GO_WALL:
		{
			Vector3 w0 = other->pos;
			Vector3 b1 = go->pos;
			Vector3 N = other->normal;

			Vector3 dir = w0 - b1;
			if (dir.Dot(N) < 0)
			{
				N = -N;
			}
			float r = go->scale.x / 2;
			float h = other->scale.x / 2;
			float l = other->scale.y / 2;
			Vector3 NP = Vector3(-N.y, N.x);

			return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
			break;
		}
		case GameObject::GO_RESOURCE1:
		{
			Vector3 w0 = other->pos;
			Vector3 b1 = go->pos;
			Vector3 N = other->normal;

			Vector3 dir = w0 - b1;
			if (dir.Dot(N) < 0)
			{
				N = -N;
			}
			float r = go->scale.x / 2;
			float h = other->scale.x / 2;
			float l = other->scale.y / 2;
			Vector3 NP = Vector3(-N.y, N.x);


			return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
			break;
		}
		case GameObject::GO_RESOURCE2:
		{
			Vector3 w0 = other->pos;
			Vector3 b1 = go->pos;
			Vector3 N = other->normal;

			Vector3 dir = w0 - b1;
			if (dir.Dot(N) < 0)
			{
				N = -N;
			}
			float r = go->scale.x / 2;
			float h = other->scale.x / 2;
			float l = other->scale.y / 2;
			Vector3 NP = Vector3(-N.y, N.x);


			return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
			break;
		}
		case GameObject::GO_RESOURCE3:
		{
			Vector3 w0 = other->pos;
			Vector3 b1 = go->pos;
			Vector3 N = other->normal;

			Vector3 dir = w0 - b1;
			if (dir.Dot(N) < 0)
			{
				N = -N;
			}
			float r = go->scale.x / 2;
			float h = other->scale.x / 2;
			float l = other->scale.y / 2;
			Vector3 NP = Vector3(-N.y, N.x);



			return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
			break;
		}
		case GameObject::GO_RESOURCE4:
		{
			Vector3 w0 = other->pos;
			Vector3 b1 = go->pos;
			Vector3 N = other->normal;

			Vector3 dir = w0 - b1;
			if (dir.Dot(N) < 0)
			{
				N = -N;
			}
			float r = go->scale.x / 2;
			float h = other->scale.x / 2;
			float l = other->scale.y / 2;
			Vector3 NP = Vector3(-N.y, N.x);



			return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
			break;
		}
		case GameObject::GO_BONUSPAD:
		{
			Vector3 w0 = other->pos;
			Vector3 b1 = go->pos;
			Vector3 N = other->normal;

			Vector3 dir = w0 - b1;
			if (dir.Dot(N) < 0)
			{
				N = -N;
			}
			float r = go->scale.x / 2;
			float h = other->scale.x / 2;
			float l = other->scale.y / 2;
			Vector3 NP = Vector3(-N.y, N.x);



			return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
			break;
		}
		case GameObject::GO_PILLAR:
		{
			Vector3 p1 = go->pos;
			Vector3 p2 = other->pos;
			float r1 = go->scale.x;
			float r2 = other->scale.x;
			Vector3 u = go->vel;
			float combinedR = r1 + r2;

			return ((p2 - p1).LengthSquared() < combinedR * combinedR && (p2 - p1).Dot(u) > 0);
		}
	}
}

void GameObject::CollisionResponse(GameObject* go2)
{
	if (this->type != GameObject::GO_BALL && go2->type != GameObject::GO_BALL)
	{
		return;
	}
	GameObject* go;
	GameObject* other;
	if (this->type != GameObject::GO_BALL)
	{
		go = go2;
		other = this;
	}
	else
	{
		go = this;
		other = go2;
	}
	Vector3 u1n, u2n;
	Vector3 N, u, uN;
	switch (other->type)
	{
	case GameObject::GO_BALL:
	{
		float m1, m2;
		m1 = go->mass;
		m2 = other->mass;

		Vector3 u1, u2;
		u1 = go->vel;
		u2 = other->vel;

		N = (other->pos - go->pos).Normalized();
		u1n = u1.Dot(N) * N;
		u2n = u2.Dot(N) * N;

		go->vel = u1 + 2 * m2 / (m1 + m2) * (u2n - u1n);
		other->vel = u2 + 2 * m1 / (m1 + m2) * (u1n - u2n);
	}
	
		break;

	case GameObject::GO_WALL:
		u = go->vel * 0.80;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		break;

	case GameObject::GO_RESOURCE1:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		(*score)++;
		Music::GetInstance()->PlayMusic(8, false, 0.5);

		(*particleGenerator).SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));

		break;
	case GameObject::GO_RESOURCE2:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		(*score)++;
		Music::GetInstance()->PlayMusic(8, false, 0.5);

		(*particleGenerator).SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));

		break;
	case GameObject::GO_RESOURCE3:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		(*score)++;
		Music::GetInstance()->PlayMusic(8, false, 0.5);

		(*particleGenerator).SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));
		break;
	case GameObject::GO_RESOURCE4:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;

		(*score)++;
		Music::GetInstance()->PlayMusic(8, false, 0.5);

		(*particleGenerator).SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));

		break;
	case GameObject::GO_BONUSPAD:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;



		break;
	case GameObject::GO_PILLAR:
	{
		Vector3 u1;
		u1 = go->vel;
		Vector3 N = (other->pos - go->pos).Normalized();
		Vector3 u1n = u1.Dot(N) * N;
		go->vel = u - 2 * u1n;
	}
		break;
	}
}

void GameObject::HandleCollision(GameObject* go2, double dt)
{
	if (this->CheckCollision(go2, dt) == true)
	{
		this->CollisionResponse(go2);
	}
}