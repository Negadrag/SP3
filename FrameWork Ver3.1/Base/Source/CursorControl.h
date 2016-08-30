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
	void Update(OrthoCamera &camera, TileMap &tileMap, const double &dt);
	int checkPositionX, checkPositionY;
	bool SpawnTower(string name);
	bool RemoveTower(Tower* tower);
	Tower* FindTower(int x,int y);
	bool bLButtonState;
	bool bPlacingTower;
	Renderable aoe;

	GUI *spawnTower[4];
	GUI *towerCosts[4];
	GUI *towerName;
	GUI *background;
	GUI *background2;
	GUI *background3;
	GUI *towerStats[T_TOTAL];
	GUI *towerUpgrades[3];
	GUI *upgradeCosts[3];
	GUI *essenceCost[3];
	GUI *selling;
	GUI *skip;

	vector<Tower*> *towerList;
	vector<Enemy*> *enemyList;
	virtual void Clear();
	bool b_warning;
private:
	string StrategyToString(Tower::STRATEGY strats);
	void TowerButtons();
	void AOEDisplay(Tower* tower);
	void HotKeys(TileMap &tileMap);
	void Clicking(TileMap &tileMap);
	void UpgradeButtons(Tower* tower);
	void SetUpgradeButtons(GUI* button,GUI* cost,GUI* ecost,string tower);
	void HandleButton(TileMap &tileMap,GUI* button);
	void CostColor(GUI* cost, Tower::ESSENCE_TYPE type);
	bool CheckPlayerEssence(Tower::ESSENCE_TYPE type, int amount);
	void RemovePlayerEssence(Tower::ESSENCE_TYPE type, int amount);

	float f_warningDebounce;
	
};

#endif