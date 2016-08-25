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
	Tower* SearchTower(vector<Tower*> towerVec);

	static int cost;

	float f_increaseATK;
	float f_increaseRANGE;
	float f_increaseRATE;

	vector<Tower*>* towerList;
private:
	vector<Tower*> buffedTowers;

};

#endif