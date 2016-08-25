#include "BuffTower.h"
#include "SingleTarget.h"

int BuffTower::cost = 5;

BuffTower::BuffTower(vector<Tower*> *tower)
:Tower()
{
	//Tower Stat
	this->i_level = 0;
	SetRange(10);
	this->atkDamage = 0;
	this->atkSpeed = 0;
	f_increaseATK = 10.f;
	f_increaseRANGE = 5.f;
	f_increaseRATE = 2.f;

	this->meshID = GEO_BUFFTOWER;
	this->fullMeshID = GEO_BUFFTOWER;
	s_name = "Buff Tower";
	this->towerList = tower;
}

BuffTower::~BuffTower()
{
	for (vector<Tower*>::iterator it = buffedTowers.begin(); it != buffedTowers.end(); ++it)
	{
		Tower* temp = *it;
		if (temp)
		{
			temp->buffCounter--;
			if (temp->buffCounter < 1)
			{
				temp->atkDamage -= f_increaseATK;
			}
		}
	}
}

vector<Tower*> BuffTower::GetTowerInRange()
{
	vector<Tower*> towerVec;
	//if (this->towerList->size() == 0)
	//{
	//	return towerVec;
	//}
	for (vector<Tower*>::iterator it = towerList->begin(); it != towerList->end(); ++it)
	{
		if ((Vector2(this->pos.x, this->pos.y) - Vector2((*it)->pos.x, (*it)->pos.y)).LengthSquared() <= this->atkRange* this->atkRange)
		{
			if ((*it)->b_isActive == true)
			{
				towerVec.push_back(*it);
			}
		}
	}
	return towerVec;
}

void BuffTower::Update(double dt)
{
	Tower::Update(dt);
	Buff();

	this->rotation.z += 50.f * dt;
}

void BuffTower::Buff()
{
	for (vector<Tower*>::iterator it = towerList->begin(); it != towerList->end(); ++it)
	{
		float d = (Vector2((*it)->pos.x, (*it)->pos.y) - Vector2(this->pos.x, this->pos.y)).LengthSquared();
		if (d < atkRange * atkRange)
		{
			Tower* temp = *it;
			if (temp != this)
			{
				bool cont = false;
				for (vector<Tower*>::iterator it2 = buffedTowers.begin(); it2 != buffedTowers.end(); ++it2)
				{
					if (temp == *it2)
					{
						cont = true;
						break;
					}
				}
				if (cont)
				{
					continue;
				}
				if (temp->buffCounter < 1)
				{
					temp->atkDamage += f_increaseATK;
				}
				temp->buffCounter++;
				buffedTowers.push_back(temp);
			}
		}
	}

}

bool BuffTower::LevelUp()
{
	return false;
}

void BuffTower::RemoveTower(Tower *tower)
{
	for (vector<Tower*>::iterator it = buffedTowers.begin(); it != buffedTowers.end();++it)
	{
		if (*it == tower)
		{
			it = buffedTowers.erase(it);
			return;
		}
	}
}