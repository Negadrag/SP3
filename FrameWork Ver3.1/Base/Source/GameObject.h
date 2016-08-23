#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Renderable.h"
#include "ParticleGenerator.h"

struct GameObject:public Renderable
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,

		GO_RESOURCE1,	//blue
		GO_RESOURCE2,	//red
		GO_RESOURCE3,	//yellow
		GO_RESOURCE4,	//green

		GO_BONUSPAD,

		GO_BLOCK,
		GO_WALL,
		GO_PILLAR,

		GO_TOTAL,
	};
	GAMEOBJECT_TYPE type;

	Vector3 vel;

	Vector3 normal;


	float mass;
	float timer;
	ParticleGenerator* particleGenerator;
	int* score;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	bool CheckCollision(GameObject* go2,double dt);
	void CollisionResponse(GameObject* go2);
	void HandleCollision(GameObject* go2,double dt);
	
	~GameObject();
};

#endif