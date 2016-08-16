#include "Node.h"

Node::Node()
{
	next = nullptr;
}

Node::Node(int x, int y)
{
	next = nullptr;
	coords.Set((float)x, (float)y);
}

Node::~Node()
{
}