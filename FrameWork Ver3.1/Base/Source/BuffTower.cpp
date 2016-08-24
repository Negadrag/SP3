#include "BuffTower.h"
#include "SingleTarget.h"

int BuffTower::cost = 5;

BuffTower::BuffTower()
:Tower()
{
	//Tower Stat
	this->i_level = 1;
	SetRange(10);
	f_increaseATK = 10.f;
	f_increaseRANGE = 5.f;
	f_increaseRATE = 2.f;

	//this->meshID = GEO_BuffTower;
	s_name = "Buff Tower";
}

BuffTower::~BuffTower()
{

}

vector<Tower*> BuffTower::GetTowerInRange()
{
	vector<Tower*> towerVec;
	if (this->towerList->size() == 0)
	{
		return towerVec;
	}
	for (vector<Tower*>::iterator it = (*towerList).begin(); it != (*towerList).end(); ++it)
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
}

void BuffTower::Buff()
{
	Tower* tower = SearchTower(GetTowerInRange());


}

Tower* BuffTower::SearchTower(vector<Tower*> towerList)
{
	Tower* tower = nullptr;
	if (towerList.empty())
	{
		return nullptr;
	}

	for (vector<Tower*>::iterator it = towerList.begin(); it != towerList.end(); ++it)
	{
		float d = (Vector2((*it)->pos.x, (*it)->pos.y) - Vector2(this->pos.x, this->pos.y)).LengthSquared();
		if (d < atkRange)
		{
			tower = (*it);
			atkDamage += f_increaseATK;
			atkRange += f_increaseRANGE;
			atkSpeed += f_increaseRATE;
		}
	}
}

void BuffTower::LevelUp()
{
	if (this->i_level >= 2)
	{
		i_level = 2;
		f_increaseATK = 15.f;
		f_increaseRANGE = 10.f;
		f_increaseRATE = 4.f;
	}

	if (atkRange > 7)
	{
		atkRange = 7;
	}
}