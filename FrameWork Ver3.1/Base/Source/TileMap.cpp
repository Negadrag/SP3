#include "TileMap.h"
#include <iostream>

TileMap::TileMap(const int tileSize)
{
	i_tileSize = tileSize;
	screenMap = nullptr;
	i_rows = i_columns = 0;
	root = nullptr;
	numCounter = 1;
}

TileMap::~TileMap()
{
	Exit();
}

bool TileMap::LoadMap(std::fstream &file)
{
	vector<string> v_rows; // temporary vector to store rows
	vector<string> wave;

	bool waveInput = false;

	if (!file.is_open())
	{
		std::cout << "File failed to open" << std::endl;
		return false;
	}
		
	while (file.good())
	{
		string temp;
		std::getline(file, temp,'\n');	
		if (temp[0] == 'e')
		{
			waveInput = true;
			continue;
		}
		if (waveInput)
		{
			wave.push_back(temp);
		}
		else
		{
			v_rows.push_back(temp);
		}
		
	}
	file.close();

	i_columns = 1; // x - axis
	for (int i = 0; i < v_rows[0].size(); ++i)
	{
		if (v_rows[0][i] == ',')
		{
			i_columns++;
		}
	}
	// SCREENMAP
	i_rows = v_rows.size(); // y-axis

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
		}
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
			}
		}
	}

	// WAVES

	waves.SetRoot(root);

	return LoadWaves(wave);
}

bool TileMap::LoadEmpty()
{
	screenMap = new int*[i_columns];
	for (int i = 0; i < i_columns; ++i)
	{
		screenMap[i] = new int[i_rows];
	}

	for (int i = 0; i < i_columns; ++i)
	{
		for (int j = 0; j < i_rows; ++j)
		{
			screenMap[i][j] = 0;
		}
	}
	return true;
}

void TileMap::AddNode(Node *node, int x, int y)
{
	if (root == nullptr)
	{
		root = new Node(x, y);
		FillPath(root, numCounter);
	}
	else if (node->next == nullptr)
	{
		node->next = new Node(x, y);
		FillPath(node, numCounter++);
	}
	else
	{
		AddNode(node->next, x, y);
	}
}

bool TileMap::FindNextNode(int number, int x, int y)
{
	for (int i = 0; i < i_columns; ++i) // x axis
	{
		if (screenMap[i][y] == number)
		{
			AddNode(root, i, y);
			return FindNextNode(number + 1, i, y);
		}
	}

	for (int j = 0; j < i_rows; ++j) // y axis
	{
		if (screenMap[x][j] == number)
		{
			AddNode(root, x, j);
			return FindNextNode(number + 1, x, j);
		}
	}

	screenMap[x][y] = 2;
	return false;
}

void TileMap::FillPath(Node *node, int nodeNumber)
{
	if (node != nullptr)
	{
		if (node->next != nullptr)
		{
			if (node->coords.x == node->next->coords.x)
			{
				if (node->coords.y < node->next->coords.y)
				{
					for (int i = node->coords.y; i < node->next->coords.y; ++i)
					{
						if (screenMap[(int)node->coords.x][i] <= 0 || (screenMap[(int)node->coords.x][i] == nodeNumber && nodeNumber != 1))
						{
							screenMap[(int)node->coords.x][i] = -1;
						}
					}
				}
				else
				{
					for (int i = node->coords.y; i > node->next->coords.y; --i)
					{
						if (screenMap[(int)node->coords.x][i] <= 0 || (screenMap[(int)node->coords.x][i] == nodeNumber && nodeNumber != 1))
						{
							screenMap[(int)node->coords.x][i] = -1;
						}
					}
				}
			}
			else if (node->coords.y == node->next->coords.y)
			{
				if (node->coords.x < node->next->coords.x)
				{
					for (int i = node->coords.x; i < node->next->coords.x; ++i)
					{
						if (screenMap[i][(int)node->coords.y] <= 0 || (screenMap[i][(int)node->coords.y] == nodeNumber && nodeNumber != 1))
						{
							screenMap[i][(int)node->coords.y] = -1;
						}
					}
				}
				else
				{
					for (int i = node->coords.x; i > node->next->coords.x; --i)
					{
						if (screenMap[i][(int)node->coords.y] <= 0 || (screenMap[i][(int)node->coords.y] == nodeNumber && nodeNumber != 1))
						{
							screenMap[i][(int)node->coords.y] = -1;
						}
					}
				}
			}
		}
	}
}

bool TileMap::LoadWaves(vector<string> wave)
{
	for (vector<string>::iterator it = wave.begin(); it != wave.end(); ++it)
	{
		std::stringstream ss(*it);

		bool checkRevolutions = false;
		bool checkFrequency = false;

		int revolutions = 0;
		float frequency = 0;

		vector<ENEMY_TYPE> enemyType;

		while (ss.good())
		{
			string temp2;
			std::getline(ss, temp2, ',');

			if (temp2 == string("/"))
			{
				checkRevolutions = true;
			}
			else if (!checkRevolutions && !checkFrequency)
			{
				if (temp2 == string("MINION")) // Taking in MINION
				{
					enemyType.push_back(MINION);
				}
				else if (temp2 == string("ICE"))
				{
					enemyType.push_back(ICE_MONSTER);
				}
				else if (temp2 == string("SPEED"))
				{
					enemyType.push_back(SPEED);
				}
				else if (temp2 == string("TANK"))
				{
					enemyType.push_back(TANK);
				}
				else if (temp2 == string("BOSS"))
				{
					enemyType.push_back(BOSS);
				}
				else // Default enemy
				{
					enemyType.push_back(MINION);
				}
			}
			else if (checkRevolutions && !checkFrequency)
			{
				revolutions = atoi(temp2.c_str());
				checkFrequency = true;
			}
			else if (checkRevolutions && checkFrequency)
			{
				frequency = atof(temp2.c_str());
				if (frequency <= 0)
				{
					frequency = 1;
				}
				break;
			}
		}

		waves.AddWave(enemyType, revolutions, frequency);
	}
	return true;
}

void TileMap::Exit()
{
	for (int i = 0; i < i_columns; ++i)
	{
		if (screenMap[i] != nullptr)
		{
			delete screenMap[i];
			screenMap[i] = nullptr;
		}
	}

	if (screenMap != nullptr)
	{
		delete screenMap;
		screenMap = nullptr;
	}

	if (root != nullptr)
	{
		delete root;
		root = nullptr;
	}

	waves.Exit();
}