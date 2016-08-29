#ifndef ICE_MONSTER_H
#define ICE_MONSTER_H

#include "Enemy.h"

class IceMonster:public Enemy
{
public:
	IceMonster();
	IceMonster(Vector3 pos, Node* root);
	~IceMonster();

	virtual void MoveTo(Vector2 dest, double dt);
	virtual void UpdateAnim(double dt);
	virtual void GiveEssence();
	virtual void ReceiveSlowStatus(float slowAmount, float slowDuration);
private:
	
protected:
	float f_floatSpeed;
	bool b_floatUp;
	float f_translateTimer;
	float f_rotateSpeed;

};

#endif