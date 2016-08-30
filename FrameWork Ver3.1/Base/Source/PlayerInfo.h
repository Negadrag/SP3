#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "WaveManager.h"
#include "OrthoCamera.h"

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

	int i_showcaseIndex;

	vector<ENEMY_TYPE> enemyToShowcase;
	bool b_showcaseEnemy;

	int m_sceneID;

	vector<ENEMY_TYPE> encounteredEnemies;

	void Init();

	OrthoCamera* tempCamera;
private:

protected:


};

#endif