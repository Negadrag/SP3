#ifndef ENEMY_H
#define ENEMY_H

#include "Renderable.h"
#include "Node.h"

class Enemy:public Renderable
{
public:
	virtual ~Enemy();
	Enemy();
	Enemy(Vector3 pos, Node* root);

	Node* nxtTile;
	int i_health;
	int i_movSpeed;
	int i_defence;//percentage of dmg mitigation
	int i_damage;
	int i_slow;// the percentage of movement speed slow

	//to move the enemy to a specific point
	void MoveTo(Vector2 dest,double dt);
	virtual void Update(double dt);
private:

protected:

};

#endif