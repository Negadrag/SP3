#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

#include "Minion.h"
#include <vector>

using std::vector;


enum ENEMY_TYPE
{
	MINION = 1,

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

	void SetRoot(Node* root);
	void AddWave(vector<ENEMY_TYPE> typeVec , int revolution , float spawnFrequency);
	bool WaveEnded(int waveIndex);
	Enemy* SpawnEnemy(ENEMY_TYPE type);
	void ClearEnemyList();
	void Update(double dt);
	vector<Enemy*>* GetEnemyList();
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