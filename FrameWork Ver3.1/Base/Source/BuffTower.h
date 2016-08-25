#ifndef BUFFTOWER_H
#define BUFFTOWER_H

#include "Tower.h"

class BuffTower : public Tower
{
public:
	BuffTower(vector<Tower*> *tower);
	~BuffTower();

	virtual void Update(double dt);
	vector<Tower*> GetTowerInRange();
	virtual bool LevelUp();
	void Buff();
	void RemoveTower(Tower *tower);

	static ESSENCE_TYPE type;
	static int cost;
	static int ecost;

	float f_increaseATK;
	float f_increaseRANGE;
	float f_increaseRATE;

	vector<Tower*>* towerList;
	vector<Tower*> buffedTowers;
};

#endif