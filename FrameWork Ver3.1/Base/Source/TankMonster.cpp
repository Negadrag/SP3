#include "TankMonster.h"
#include "PlayerInfo.h"

TankMonster::TankMonster() :Enemy()
{
}

TankMonster::TankMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_TANKY;
	this->f_movSpeed = 1.5f;
	this->f_maxHealth = 100.f;
	this->f_health = f_maxHealth;

	this->i_damage = 1;
	this->i_defence = 30;
	this->f_rotateSpeed = 90.f;
	i_currency = 2;
	this->scale.Set(0.75f, 0.75f, 0.75f);
}

TankMonster::~TankMonster()
{
}

void TankMonster::UpdateAnim(double dt)
{
}

void TankMonster::GiveEssence()
{
	//Enemy::GiveCurrency();
	player->i_essenceTanky += 1;
}