#ifndef TOWER_H
#define TOWER_H

#include "Projectile.h"
#include <vector>
#include "Renderable.h"
#include "Enemy.h"
using std::vector;

class Tower : public Renderable
{
public:
	virtual ~Tower();

	enum STRATEGY
	{
		FIRST_ENEMY=0,
		NEAREST_ENEMY,
		FURTHER_ENEMY,
		LOWEST_HEALTH,
		HIGHEST_HEALTH,

		NUM_STRATEGY
	};
	STRATEGY strategy;
	void SetType(GEOMETRY_TYPE meshID);
	void SetCost(float c);
	void SetAtkDmg(float ad);
	void SetSpdRate(float sr);
	void SetRange(float r);
	float GetRange();
	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual void Fire();
	void ClearProjectile();
	vector<Enemy*> GetEnemyInRange();

	Enemy* SearchEnemy(vector<Enemy*> enemyVec);

	bool isinRange;
	vector<Projectile* > projectileList;
	
	Vector3 heightOffset;
	vector<Enemy*>* enemyList;

	string s_name;
private:

protected:
	Tower();
	Tower(Vector3 pos, Vector3 scale, Vector3 heightOffset);

	GEOMETRY_TYPE projectile_meshID;
	
	int i_level;
	float p_frequency;
	float p_spawnTimer;
	float towerCost;
	float atkDamage;
	float atkSpeed;
	float atkRange;
	float p_speed;
	float newAngle;
	int p_projectileCount;
	int p_maxProjectile;

};
#endif

