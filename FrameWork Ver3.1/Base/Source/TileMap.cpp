#include "TileMap.h"
#include <iostream>

TileMap::TileMap(const int tileSize)
{
	i_tileSize = tileSize;
	screenMap = nullptr;
	i_rows = i_columns = 0;
	root = nullptr;
}

TileMap::~TileMap()
{
	for (int i = 0; i < i_columns; ++i)
	{
		if (screenMap[i] != nullptr)
		{
			delete screenMap[i];
		}
	}

	if (screenMap != nullptr)
	{
		delete screenMap;
	}

	if (root != nullptr)
	{
		delete root;
	}
}

bool TileMap::LoadMap(std::fstream &file)
{
	vector<string> v_rows; // temporary vector to store rows
		
	while (file.good())
	{
		string temp;
		std::getline(file, temp,'\n');	
		v_rows.push_back(temp);
	}
	file.close();

	i_columns = v_rows[0].size() - 1; // x-axis
	i_rows = v_rows.size() - 1; // y-axis

	screenMap = new int*[i_columns];
	for (int i = 0; i < i_columns; ++i)
	{
		screenMap[i] = new int[i_rows];
	}

	// Writing the values into the tilemap
	int k = 0;
	for (int j = i_rows - 1; j >= 0; --j)
	{
		std::stringstream ss(v_rows[j]);
		for (int i = 0; i < i_columns; ++i)
		{
			string temp2;
			std::getline(ss, temp2, ',');
			int temp3 = std::atoi(temp2.c_str());
			screenMap[i][k] = temp3;
			std::cout << temp3;
		}
		std::cout << std::endl;
		k++;
	}

	// Starting the recursive node loop
	for (int j = i_rows - 1; j >= 0; --j) // y axis
	{
		for (int i = 0; i < i_columns; ++i) // x axis
		{
			if (screenMap[i][j] == 1)
			{
				AddNode(root, i, j);
				FindNextNode(2, i, j);
				return true;
			}
		}
	}
	return true;
}

void TileMap::AddNode(Node *node, int x, int y)
{
	if (root == nullptr)
	{
		root = new Node(x, y);
	}
	else if (node->next == nullptr)
	{
		node->next = new Node(x, y);
	}
	else
	{
		AddNode(node->next, x, y);
	}
}

void TileMap::FindNextNode(int number, int x, int y)
{
	for (int i = 0; i < i_columns; ++i) // x axis
	{
		if (screenMap[i][y] == number)
		{
			AddNode(root, i, y);
			FindNextNode(number + 1, i, y);
			return;
		}
	}

	for (int j = 0; j < i_rows; ++j) // y axis
	{
		if (screenMap[x][j] == number)
		{
			AddNode(root, x, j);
			FindNextNode(number + 1, x, j);
			return;
		}
	}
}