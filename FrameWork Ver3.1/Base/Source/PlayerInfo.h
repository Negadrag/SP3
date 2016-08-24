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

	vector<ENEMY_TYPE> encounteredEnemies;

	void Init();


private:

protected:


};

#endif