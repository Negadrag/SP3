#ifndef MINION_H
#define MINION_H

#include "Enemy.h"

class Minion : public Enemy
{
public:
	Minion();
	Minion(Vector3 pos, Node* node);
	virtual ~Minion();

private:

protected:

};

#endif