#include "WaveManager.h"
#include "Minion.h"
#include "IceMonster.h"
#include "SpeedMonster.h"
#include "TankMonster.h"
#include "PlayerInfo.h"
#include "SceneManager.h"

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
	f_startingHp = 90.f;
	f_hpScaling = 10.f; 
	f_currScaling = f_startingHp;


	this->root = root;
	this->b_allWaveEnded = false;
	this->player = nullptr;
	b_miniGame = true;
}

WaveManager::~WaveManager()
{
	Exit();
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
			f_waveStartTimer += (float)dt;
			if (f_waveStartTimer >= 0.9f)
			{
				b_waveEnded = true;
			}
			if ( ( (i_currentWave)% 5 == 0) && i_currentWave>0 && f_waveStartTimer >= 2.25f && b_miniGame == true && i_currentWave < waveList.size())
			{
				b_miniGame = false;
				player->m_sceneID = SceneManager::GetInstance()->m_currentSceneID;
				SceneManager::GetInstance()->ChangeScene(3,true);
			}
			if (f_waveStartTimer >= 10.f)
			{
				f_currScaling *= (1.f + (f_hpScaling / 100.f));
				player->i_currency += 5;
				b_miniGame = true;
				player->i_currency += 5;
				f_waveStartTimer = 0.f;
				i_currentRevolution = 0;
				i_currentWave++;
				b_waveEnded = false;
				if (i_currentWave >= waveList.size())
				{
					b_allWaveEnded = true;
				}
				else
				{
					player->enemyToShowcase.clear();
					for (vector<ENEMY_TYPE>::iterator it = waveList[i_currentWave].typeVec.begin(); it != waveList[i_currentWave].typeVec.end(); ++it)
					{
						bool haveEncountered = false;
						for (vector<ENEMY_TYPE>::iterator it2 = player->encounteredEnemies.begin(); it2 != player->encounteredEnemies.end(); ++it2)
						{
							if (*it == *it2)
							{
								haveEncountered = true;
							}
						}
						if (haveEncountered == false)
						{
							player->enemyToShowcase.push_back(*it);
							player->encounteredEnemies.push_back(*it);
						}
					}

					if (player->enemyToShowcase.empty() == false)
					{
						player->m_sceneID = SceneManager::GetInstance()->m_currentSceneID;
						player->b_showcaseEnemy = true;
						player->i_showcaseIndex = 0;
						SceneManager::GetInstance()->ChangeScene(7, true);//change to display Scene;
					}
				}

			}
		}
		else
		{
			f_waveStartTimer = 0.f;
		}

		if (b_waveEnded == true)
		{
			ClearEnemyList();
			if (player->enemyToShowcase.size() > 0)
			{
				player->m_sceneID = SceneManager::GetInstance()->m_currentSceneID;
				player->b_showcaseEnemy = false;
				player->i_showcaseIndex = 0;
				SceneManager::GetInstance()->ChangeScene(7, true);//change to display Scene;x`
				
			}
		}
	}
	if (b_allWaveEnded)
	{
		SceneManager::GetInstance()->ChangeScene(5, false);
	}

}

void WaveManager::ClearEnemyList()
{
	if (enemyList.empty())
	{
		return;
	}
	for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		delete (*it);
	}
	enemyList.clear();
}

void WaveManager::StartWave()
{
	f_waveStartTimer = 10.f;
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

		case ICE_MONSTER:
		{
			enemy = new IceMonster(pos, root);
		}
			break;
		case SPEED:
		{
			enemy = new SpeedMonster(pos, root);
		}
			break;
		case TANK:
		{
			enemy = new TankMonster(pos, root);
		}
			break;
		case NUM_ENEMY:
			break;
		default:
			break;
	}
	if (enemy != nullptr)
	{
		enemy->player = this->player;
		enemy->f_health *= (f_currScaling/100.f);
		enemy->f_maxHealth *= (f_currScaling / 100.f);
		enemyList.push_back(enemy);
	}

	return enemy;
}

vector<Enemy*>* WaveManager::GetEnemyList()
{
	return &enemyList;
}

void WaveManager::Exit()
{
	f_spawnTimer = 0.f;
	i_currentWave = 0;
	i_currentRevolution = 0;
	i_typeVecIndex = 0;
	f_waveStartTimer = 0.f;

	this->root = nullptr;
	this->b_allWaveEnded = false;
	this->player = nullptr;
	b_miniGame = true;

	ClearEnemyList();
	waveList.clear();
}