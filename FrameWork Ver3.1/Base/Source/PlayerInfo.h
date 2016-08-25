#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "WaveManager.h"

class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();

	int i_health;
	int i_currency;

	int i_essenceBasic;
	int i_essenceSpeed;
	int i_essenceTanky;
	int i_essenceIce;

	ENEMY_TYPE enemyToShowcase;
	bool b_showcaseEnemy;

	vector<ENEMY_TYPE> encounteredEnemies;

	void Init();


private:

protected:


};

#endif