#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Node.h"
#include "WaveManager.h"

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
	WaveManager waves;

	bool LoadMap(std::fstream &file);
	bool LoadWaves(vector<string> wave);
	bool LoadEmpty();
	void AddNode(Node *node,int x, int y);
	bool FindNextNode(int number, int x, int y);
	void FillPath(Node *node, int nodeNumber);
private:
	int numCounter;
};

#endif