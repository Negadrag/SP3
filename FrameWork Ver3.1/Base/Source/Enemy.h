#ifndef ENEMY_H
#define ENEMY_H

#include "Renderable.h"
#include "Node.h"


class PlayerInfo;

class Enemy:public Renderable
{
public:
	virtual ~Enemy();


	Node* nxtTile;
	float f_movSpeed;
	float f_health;
	
	PlayerInfo* player;

	int i_currency;

	//to move the enemy to a specific point handles rotation based on vel as well. Rotation speed set within the function
	virtual void MoveTo(Vector2 dest,double dt);
	void Update(double dt);
	void ReceiveDamage(int damage);
	void ReceiveSlowStatus(float slowAmount, float slowDuration);
	void ReceivePoisonStatus(float poisonDamage,float slowAmount,float duration);
	virtual void UpdateAnim(double dt);
	virtual void GiveCurrency();
	virtual void GiveEssence();
private:

protected:
	//int i_health;
	
	int i_defence;//percentage of dmg mitigation
	int i_damage;
	float f_slow;// the percentage of movement speed slow
	float f_poisonDps;
	float f_slowTimer;
	float f_poisonTimer;
	

	Enemy();
	Enemy(Vector3 pos, Node* root);
};

#endif