#include "Tower.h"

Tower::Tower()
: Renderable()
{
	i_level = 1;
	p_spawnTimer = 0.f;
	towerCost = 0.f;
	atkDamage = 0.f;
	atkSpeed = 0.f;
	atkRange = 0.f;
	p_speed = 10.0f;
	p_projectileCount = 0;
	p_maxProjectile = 0;
	this->pos.SetZero();
	enemyList = nullptr;
	heightOffset.SetZero();
	strategy = NEAREST_ENEMY;
	s_name = "";
	this->b_rotateWhenFire = true;
	this->f_rotationToBe = 0.f;
	child.meshID = GEO_NULL;
	child.SetParent(this);
}

Tower::Tower(Vector3 pos, Vector3 scale, Vector3 heightOffset)
: Renderable()
{
	i_level = 1;
	p_spawnTimer = 0.f;
	this->pos = pos;
	this->scale = scale;
	towerCost = 0.f;
	atkDamage = 0.f;
	atkSpeed = 0.f;
	atkRange = 0.f;
	p_projectileCount = 0;
	p_maxProjectile = 0;
	heightOffset.Set(0, 0, 5);
	p_speed = 10.0f;
	enemyList = nullptr;
	strategy = FIRST_ENEMY;
	s_name = "";
	this->b_rotateWhenFire = true;
	this->f_rotationToBe = 0.f;
	child.meshID = GEO_NULL;
	child.SetParent(this);
}

Tower::~Tower()
{
	for (vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		delete(*it);
	}
}

void Tower::SetType(GEOMETRY_TYPE meshID)
{
	this->meshID = meshID;
}

void Tower::SetCost(float c)
{
	this->towerCost = c;
}

float Tower::GetCost()
{
	return towerCost;
}

void Tower::SetAtkDmg(float ad)
{
	this->atkDamage = ad;
}

float Tower::GetAtkDmg()
{
	return atkDamage;
}

void Tower::SetSpdRate(float sr)
{
	this->atkSpeed = sr;
}

float Tower::GetSpdRate()
{
	return atkSpeed;
}

void Tower::SetRange(float r)
{
	this->atkRange = r;
}

float Tower::GetRange()
{
	return atkRange;
}


Projectile* Tower::GetProjectile()
{
	for (std::vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)(*it);
		if (!(projectile->b_isActive))
		{
				projectile->b_isActive = true;
				projectile->meshID = projectile_meshID;
				return projectile;

		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{
		Projectile* projectile = new Projectile(projectile_meshID);
		projectile->b_isActive = false;
		projectileList.push_back(projectile);
	}
	projectileList.back()->b_isActive = true;
	return projectileList.back();
}

void Tower::Update(double dt)
{

	p_spawnTimer += (float)dt;
	if (p_spawnTimer >= 1.f /this->atkSpeed)// p_frequency) && p_projectileCount<p_maxProjectile)
	{
		p_spawnTimer = 0.f;
		Fire(dt);
	}
	if (b_rotateWhenFire == true && child.meshID != GEO_NULL)
	{
		float rotationSpeed = 450.f;
		if (child.rotation.z != f_rotationToBe)// to rotate the model if the enemy is turning
		{
			if (f_rotationToBe < 0) // making sure f_rotationToBe is withiin 0 and 360
			{
				f_rotationToBe += 360;
			}
			else if (f_rotationToBe == -0)
			{
				f_rotationToBe = 0;
			}

			if (abs(f_rotationToBe - child.rotation.z) <180)
			{
				if (f_rotationToBe > child.rotation.z)
				{

					child.rotation.z += rotationSpeed * dt;
					if (f_rotationToBe < child.rotation.z)
					{
						child.rotation.z = f_rotationToBe;
					}


				}
				else if (f_rotationToBe < child.rotation.z)
				{
					child.rotation.z -= rotationSpeed * dt;
					if (f_rotationToBe > child.rotation.z)
					{
						child.rotation.z = f_rotationToBe;
					}
				}
			}
			else
			{
				if (f_rotationToBe > child.rotation.z)
				{

					child.rotation.z -= rotationSpeed * dt;
					if (child.rotation.z < 0.f) // wind around if negative numbers
					{
						child.rotation.z += 360;
					}
					else if (child.rotation.z == -0.f) // negate negative 0;
					{
						child.rotation.z = 0;
					}

				}
				else if (f_rotationToBe < child.rotation.z)
				{
					child.rotation.z += rotationSpeed * dt;
					if (child.rotation.z >360.f)
					{
						child.rotation.z -= 360;
					}
				}
			}
		}
	}
}

void Tower::Fire(double dt)
{
	Enemy* enemy = SearchEnemy(GetEnemyInRange());
	if (enemy == nullptr || enemy->b_isActive == false)
	{
		f_rotationToBe = 270.f;
		return;
	}
	if (b_rotateWhenFire == true)
	{
		Vector3 view = enemy->pos - this->pos;
		f_rotationToBe = Math::RadianToDegree(atan2(view.y, view.x)); // the rotation that we want it to be at;

		f_rotationToBe = round(f_rotationToBe);


	}
	Projectile* projectile = GetProjectile();
	projectile->meshID = this->projectile_meshID;
	projectile->pos = this->pos + heightOffset;

	if (projectile->meshID == GEO_ARROW || projectile->meshID == GEO_POISONARROW || projectile->meshID == GEO_ICESHOT)
		projectile->scale.Set(2, 2, 2);
	else
		projectile->scale.Set(0.5f, 0.5f, 0.5f);

	projectile->p_speed = this->p_speed;
	projectile->enemy = enemy;
	projectile->vel = (enemy->pos - projectile->pos).Normalize() * p_speed;
	projectile->i_damage = this->atkDamage;

	projectileList.push_back(projectile);
	//enemy->i_health -= 1;
}

void Tower::ClearProjectile()
{
	for (vector<Projectile*>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		delete(*it);
	}
	projectileList.clear();
}

vector<Enemy*> Tower::GetEnemyInRange()
{
	vector<Enemy*> enemyVec;
	if (this->enemyList->size() == 0)
	{
		return enemyVec;
	}
	for (vector<Enemy*>::iterator it = (*enemyList).begin(); it != (*enemyList).end(); ++it)
	{
		if ((Vector2(this->pos.x, this->pos.y) - Vector2((*it)->pos.x, (*it)->pos.y)).LengthSquared() <= this->atkRange * this->atkRange)
		{
			if ((*it)->b_isActive == true)
			{
				enemyVec.push_back((*it));
			}
			
		}
	}
	return enemyVec;
}

Enemy* Tower::SearchEnemy(vector<Enemy*> enemyList)
{
	Enemy* enemy = nullptr;
	if (enemyList.empty())
	{
		return nullptr;
	}

	if (strategy == FIRST_ENEMY)
	{
		Node* furthestNode = (*enemyList.begin())->nxtTile;
		for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			Node* checker = (*it)->nxtTile;
			bool isAhead = true;
			while(checker != nullptr)
			{
				if (checker == furthestNode)
				{
					isAhead = false;
				}
				checker = checker->next;
			}
			if (isAhead == true)
			{
				furthestNode = (*it)->nxtTile;
			}
		}
		float shortestDist = FLT_MAX;
		if (furthestNode == nullptr)
		{
			return nullptr;
		}
		for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			if ((*it)->nxtTile == furthestNode)
			{
				float d = (Vector2((*it)->pos.x, (*it)->pos.y) - furthestNode->coords).LengthSquared();
				if (d < shortestDist)
				{
					shortestDist = d;
					enemy = (*it);
				}
			}
		}
	}

	else if (strategy == NEAREST_ENEMY)
	{
		float shortestDist = FLT_MAX;
		for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{

				float d = (Vector2((*it)->pos.x, (*it)->pos.y) - Vector2(this->pos.x, this->pos.y)).LengthSquared();
				if (d < shortestDist)
				{
					shortestDist = d;
					enemy = (*it);
				}

		}
	}

	else if (strategy == FURTHER_ENEMY)
	{
		float furthestDist = 0;
		for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{

			float d = (Vector2((*it)->pos.x, (*it)->pos.y) - Vector2(this->pos.x, this->pos.y)).LengthSquared();
			if (d > furthestDist)
			{
				furthestDist = d;
				enemy = (*it);
			}

		}
	}

	else if (strategy == LOWEST_HEALTH)
	{
		int lowestHP = INT_MAX;
		for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			int hp = (*it)->f_health;
			if (hp < lowestHP)
			{
				lowestHP = hp;
				enemy = (*it);
			}

		}

	}
	else if (strategy == HIGHEST_HEALTH)
	{
		int highestHP = 0;
		for (vector<Enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			int hp = (*it)->f_health;
			if (hp > highestHP)
			{
				highestHP = hp;
				enemy = (*it);
			}

		}
	}


	return enemy;
}

void Tower::LevelUp()
{

}

string Tower::StrategyToString(STRATEGY strats)
{
	if (strats == FIRST_ENEMY)
	{
		return string("First Enemy");
	}
	else if (strats == NEAREST_ENEMY)
	{
		return string("Nearest Enemy");
	}
	else if (strats == FURTHER_ENEMY)
	{
		return string("Furthest Enemy");
	}
	else if (strats == LOWEST_HEALTH)
	{
		return string("Lowest Health");
	}
	else if (strats == HIGHEST_HEALTH)
	{
		return string("Highest Health");
	}
}