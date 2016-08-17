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
	Tower();
	Tower(Vector3 pos, Vector3 scale, Vector3 heightOffset);
	~Tower();

	enum TOWERUPGRADE
	{
		T_TOWER1,
		T_TOWER2,
		NUM_TOWERUPGRADE
	};
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
	TOWERUPGRADE level;
	void SetType(GEOMETRY_TYPE meshID);
	void SetCost(float c);
	void SetAtkDmg(float ad);
	void SetSpdRate(float sr);
	void SetRange(float r);
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
private:

protected:
	GEOMETRY_TYPE projectile_meshID;
	
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

