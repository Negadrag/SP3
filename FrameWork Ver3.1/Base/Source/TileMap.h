#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Node.h"

using std::string;
using std::vector;


class TileMap
{
public:
	TileMap(const int tileSize = 1);
	~TileMap();

	int **screenMap; // screenMap[x][y] , x from left and y from bottom
	int i_tileSize;
	int i_rows, i_columns;

	Node *root;

	bool LoadMap(std::fstream &file);
	void AddNode(Node *node,int x, int y);
	void FindNextNode(int number, int x, int y);
};

#endif