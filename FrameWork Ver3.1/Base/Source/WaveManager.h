#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

#include "Enemy.h"
#include <vector>

using std::vector;

class PlayerInfo;

enum ENEMY_TYPE
{
	MINION = 1,
	ICE_MONSTER,
	SPEED,
	TANK,
	BOSS,
	NUM_ENEMY
};

struct Wave
{
public:

	Wave(vector<ENEMY_TYPE> typeVec = {}, int revolution = 0, float spawnFrequency = 0);
	~Wave();

	vector<ENEMY_TYPE>  typeVec;
	
	int i_revolution;
	

	float f_spawnFrequency;
private:

protected:

};

class WaveManager
{
public:
	WaveManager(Node* root = nullptr);
	~WaveManager();

	bool b_allWaveEnded;
	float f_waveStartTimer;
	int i_typeVecIndex;
	bool b_waveEnded;
	bool b_miniGame;
	float f_startingHp; // the starting percentage of the monster base hp for wave 1
	float f_hpScaling; // percentage of the health scaling per wave;
	float f_currScaling; // percentage scaling of the current wave

	void SetRoot(Node* root);
	void AddWave(vector<ENEMY_TYPE> typeVec , int revolution , float spawnFrequency);
	bool WaveEnded(int waveIndex);
	void StartWave();
	Enemy* SpawnEnemy(ENEMY_TYPE type);
	void ClearEnemyList();
	void Exit();
	void Update(double dt);
	vector<Enemy*>* GetEnemyList();
	PlayerInfo* player;
private:
	vector<Enemy*> enemyList;
	vector<Wave> waveList;
	
	Node* root;
	float f_spawnTimer;
	int i_currentWave;
	int i_currentRevolution;
protected:


};
#endif