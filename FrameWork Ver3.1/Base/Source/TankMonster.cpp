#include "TankMonster.h"
#include "PlayerInfo.h"

TankMonster::TankMonster() :Enemy()
{
	this->meshID = GEO_TANKY;
	this->f_movSpeed = 1.5f;
	this->f_health = 100;
	this->i_damage = 1;
	this->i_defence = 30;
	this->f_rotateSpeed = 90.f;
	this->i_currency = 1;
}

TankMonster::TankMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_TANKY;
	this->f_movSpeed = 1.5f;
	this->f_health = 100;

	this->i_damage = 1;
	this->i_defence = 30;
	this->f_rotateSpeed = 90.f;
	i_currency = 1;
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