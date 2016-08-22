#ifndef NODE_H
#define NODE_H

#include "Vector2.h"

class Node
{
public:
	Node();
	Node(int x, int y);
	~Node();
	Vector2 coords;
	Node *next;
	int i_number;
};

#endif