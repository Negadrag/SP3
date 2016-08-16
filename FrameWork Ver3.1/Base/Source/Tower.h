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

	void SetType(GEOMETRY_TYPE meshID);
	void SetCost(float c);
	void SetAtkDmg(float ad);
	void SetSpdRate(float sr);
	void SetRange(float r);
	Projectile* GetProjectile();
	virtual void Update(double dt);
	void Fire();
	void ClearProjectile();

	bool isinRange;
	vector<Projectile* > projectileList;
	
	Vector3 heightOffset;
	Enemy* enemy;
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

