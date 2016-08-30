#ifndef SPEED_MONSTER_H
#define SPEED_MONSTER_H

#include "Enemy.h"

class SpeedMonster :public Enemy
{
public:
	SpeedMonster();
	SpeedMonster(Vector3 pos, Node* root);
	~SpeedMonster();

	virtual void UpdateAnim(double dt);
	virtual void GiveEssence();
	virtual void UpdateMesh();

private:

protected:
	float f_rotateSpeed;

};

#endif