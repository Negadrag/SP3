#include "Node.h"

Node::Node()
{
	next = nullptr;
	i_number = 0;
}

Node::Node(int x, int y)
{
	next = nullptr;
	coords.Set((float)x, (float)y);
	i_number = 0;
}

Node::~Node()
{
	if (this->next != nullptr)
	{
		delete this->next;
	}
}