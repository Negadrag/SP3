#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"

class Tower;

class Boss :public Enemy
{
public:
	Boss();
	Boss(Vector3 pos, Node* root);
	~Boss();

	bool b_onGround;
	Vector2 targetPos;
	float zVel;

	vector<Tower*>* towerList;

	virtual void MoveTo(Vector2 dest, double dt);
	virtual void Update(double dt);
	virtual void UpdateAnim(double dt);
	virtual void GiveEssence();
private:

protected:

};

#endif