#ifndef TOWER_H
#define TOWER_H

#include "Projectile.h"
#include <vector>
#include "Renderable.h"
#include "Enemy.h"
#include "Music.h"

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

	enum ESSENCE_TYPE
	{
		E_BASIC,
		E_SPEED,
		E_ICE,
		E_HEAVY,
		E_TOTAL
	};

	STRATEGY strategy;
	void SetType(GEOMETRY_TYPE meshID);
	void SetCost(float c);
	float GetCost();
	void SetAtkDmg(float ad);
	float GetAtkDmg();
	void SetSpdRate(float sr);
	float GetSpdRate();
	void SetRange(float r);
	float GetRange();
	virtual Projectile* GetProjectile();
	virtual void Update(double dt);
	virtual void Fire(double dt);
	void ClearProjectile();
	vector<Enemy*> GetEnemyInRange();
	virtual bool LevelUp();

	static string StrategyToString(STRATEGY strats);

	Enemy* SearchEnemy(vector<Enemy*> enemyVec);

	bool isinRange;
	vector<Projectile* > projectileList;
	
	Vector3 heightOffset;
	vector<Enemy*>* enemyList;
	Renderable child;

	GEOMETRY_TYPE fullMeshID;

	string s_name;
	int i_level;
	int i_MaxLevel;

	string upgrades[2];

	float atkDamage;
	float atkSpeed;
	float atkRange;
	int buffCounter;

	ESSENCE_TYPE essence;
	float essenceCost;
private:

protected:
	Tower();
	Tower(Vector3 pos, Vector3 scale, Vector3 heightOffset);

	GEOMETRY_TYPE projectile_meshID;
	
	float p_frequency;
	float p_spawnTimer;
	float towerCost;
	

	float p_speed;
	float newAngle;
	int p_projectileCount;
	int p_maxProjectile;
	//for tower rotation when firing
	bool b_rotateWhenFire;
	float f_rotationToBe;//desired rotation calculated in fire function
};
#endif

