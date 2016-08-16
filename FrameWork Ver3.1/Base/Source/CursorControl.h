#ifndef CURSORCONTROL_H
#define CURSORCONTROL_H

#include "Camera.h"
#include "TileMap.h"
#include "Tower.h"

class CursorControl
{
public:
	CursorControl();
	~CursorControl();
	void Init(vector<Tower*> *towerList,vector<Enemy*> *enemyList);
	void Update(const Camera &camera, const TileMap &tileMap);
	int checkPositionX, checkPositionY;
	bool SpawnTower();

	vector<Tower*> *towerList;
	vector<Enemy*> *enemyList;
};

#endif