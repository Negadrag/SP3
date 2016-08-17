#ifndef ENEMY_H
#define ENEMY_H

#include "Renderable.h"
#include "Node.h"

class Enemy:public Renderable
{
public:
	virtual ~Enemy();


	Node* nxtTile;
	float f_movSpeed;
	int i_health;

	//to move the enemy to a specific point handles rotation based on vel as well. Rotation speed set within the function
	void MoveTo(Vector2 dest,double dt);
	void Update(double dt);
	void ReceiveDamage(int damage);
private:

protected:
	//int i_health;
	
	int i_defence;//percentage of dmg mitigation
	int i_damage;
	int i_slow;// the percentage of movement speed slow
	Enemy();
	Enemy(Vector3 pos, Node* root);
};

#endif