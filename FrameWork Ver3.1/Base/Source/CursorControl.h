#ifndef CURSORCONTROL_H
#define CURSORCONTROL_H

#include "Cursor.h"
#include "Tower.h"
#include "GUI.h"

class CursorControl : public Cursor
{
public:
	enum TOWER_STATS
	{
		T_ATK,
		T_SPEED,
		T_STRATEGY,
		T_TOTAL,
	};

	CursorControl();
	~CursorControl();
	void Init(vector<Tower*> *towerList, vector<Enemy*> *enemyList);
	void Update(OrthoCamera &camera, const TileMap &tileMap, const double &dt);
	int checkPositionX, checkPositionY;
	bool SpawnTower(string name);
	Tower* FindTower(int x,int y);
	bool bLButtonState;
	Renderable aoe;

	GUI *spawnTower[4];
	GUI *towerCosts[4];
	GUI *towerName;
	GUI *background;
	GUI *background2;
	GUI *towerStats[T_TOTAL];

	vector<Tower*> *towerList;
	vector<Enemy*> *enemyList;
private:
	string StrategyToString(Tower::STRATEGY strats);
	void TowerButtons();
	void AOEDisplay(Tower* tower);
	void HotKeys(const TileMap &tileMap);
	void Clicking(const TileMap &tileMap);
};

#endif