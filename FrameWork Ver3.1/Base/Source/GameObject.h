#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Renderable.h"

struct GameObject:public Renderable
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,

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

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif