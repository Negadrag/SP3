#ifndef CURSORCONTROL_H
#define CURSORCONTROL_H

#include "OrthoCamera.h"
#include "TileMap.h"
#include "Tower.h"
#include "MatrixStack.h"
#include "RenderManager.h"

class CursorControl
{
public:
	CursorControl();
	~CursorControl();
	void Init(vector<Tower*> *towerList, vector<Enemy*> *enemyList);
	void Update(const OrthoCamera &camera, const TileMap &tileMap);
	int checkPositionX, checkPositionY;
	Vector3 worldCoords;
	bool SpawnTower(string name);
	Tower* FindTower(int x,int y);

	vector<Tower*> *towerList;
	vector<Enemy*> *enemyList;
};

#endif