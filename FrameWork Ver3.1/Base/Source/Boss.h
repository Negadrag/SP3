#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"

class Boss :public Enemy
{
public:
	Boss();
	Boss(Vector3 pos, Node* root);

	/*virtual void MoveTo(Vector2 dest, double dt);
	virtual void Update(double dt);
	virtual void UpdateAnim(double dt);
	virtual void GiveCurrency();
	virtual void GiveEssence();*/
private:

protected:

};

#endif