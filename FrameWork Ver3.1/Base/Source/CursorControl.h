#ifndef CURSORCONTROL_H
#define CURSORCONTROL_H

#include "Cursor.h"
#include "Tower.h"
#include "GUI.h"

class CursorControl : public Cursor
{
public:
	CursorControl();
	~CursorControl();
	void Init(vector<Tower*> *towerList, vector<Enemy*> *enemyList);
	void Update(OrthoCamera &camera, const TileMap &tileMap, const double &dt);
	int checkPositionX, checkPositionY;
	bool SpawnTower(string name);
	Tower* FindTower(int x,int y);
	bool bLButtonState;
	Renderable aoe;

	GUI spawnTower[4];
	GUI towerCosts[4];
	GUI towerName;

	vector<Tower*> *towerList;
	vector<Enemy*> *enemyList;
private:
	void TowerButtons(float worldX,float worldY);
	void AOEDisplay(Tower* tower);
	void HotKeys(const TileMap &tileMap);
};

#endif