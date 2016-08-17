#include "WaveManager.h"

Wave::Wave(vector<ENEMY_TYPE> typeVec,int revolution, float spawnFrequency)
{
	this->typeVec = typeVec;
	this->i_revolution = revolution;
	this->f_spawnFrequency = spawnFrequency;
}

Wave::~Wave()
{
}



WaveManager::WaveManager(Node* root)
{
	f_spawnTimer = 0.f;
	i_currentWave = 0;
	i_currentRevolution = 0;
	i_typeVecIndex = 0;
	f_waveStartTimer = 0.f;

	this->root = root;
	this->b_allWaveEnded = false;
}

WaveManager::~WaveManager()
{
	ClearEnemyList();
}

void WaveManager::SetRoot(Node* root)
{
	this->root = root;
}

void WaveManager::AddWave(vector<ENEMY_TYPE> typeVec, int revolution, float spawnFrequency)
{
	Wave theWave(typeVec, revolution, spawnFrequency);
	waveList.push_back(theWave);
}

bool WaveManager::WaveEnded(int waveIndex)
{
	if (waveIndex >= waveList.size())
	{
		b_allWaveEnded = true;
		return true;
	}
	if (i_currentRevolution != waveList[waveIndex].i_revolution)
	{
		return false;
	}
	else
	{
		for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			if ((*it)->b_isActive == true)
			{
				return false;
			}
		}
		return true;
	}
}

void WaveManager::Update(double dt)
{
	if (b_allWaveEnded == false)
	{
		f_spawnTimer += (float)dt;
		if (f_spawnTimer >= (1.f / waveList[i_currentWave].f_spawnFrequency) )
		{
			f_spawnTimer = 0.f;
			if (i_currentRevolution != waveList[i_currentWave].i_revolution)
			{
				SpawnEnemy(waveList[i_currentWave].typeVec[i_typeVecIndex]);
				i_typeVecIndex++;
				if (i_typeVecIndex >= waveList[i_currentWave].typeVec.size())
				{
					i_currentRevolution++;
					i_typeVecIndex = 0;
				}
			}
		}
		else if (WaveEnded(i_currentWave))
		{
			f_waveStartTimer += dt;
			if (f_waveStartTimer >= 5.f)
			{
				f_waveStartTimer = 0.f;
				ClearEnemyList();
				i_currentRevolution = 0;
				i_currentWave++;
				if (i_currentWave >= waveList.size())
				{
					b_allWaveEnded = true;
				}
			}
			
		}
	}
}

void WaveManager::ClearEnemyList()
{
	for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		delete (*it);
	}
	enemyList.clear();
}

Enemy* WaveManager::SpawnEnemy(ENEMY_TYPE type)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	Enemy* enemy = nullptr;
	Vector3 pos(root->coords.x, root->coords.y, 1.f);
	switch (type)
	{
		case MINION:
		{
			enemy = new Minion(pos,root);
		}
			break;
		case NUM_ENEMY:
			break;
		default:
			break;
	}
	if (enemy != nullptr)
	{
		enemyList.push_back(enemy);
	}
	return enemy;
}

vector<Enemy*>* WaveManager::GetEnemyList()
{
	return &enemyList;
}