#include "TankMonster.h"
#include "PlayerInfo.h"

TankMonster::TankMonster() :Enemy()
{
	this->meshID = GEO_TANKY;
	this->f_movSpeed = 3.f;
	this->f_health = 100;
	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	this->i_currency = 1;
}

TankMonster::TankMonster(Vector3 pos, Node* root) :Enemy(pos, root)
{
	this->meshID = GEO_TANKY;
	this->f_movSpeed = 3.f;
	this->f_health = 50;

	this->i_damage = 1;
	this->i_defence = 0;
	this->f_rotateSpeed = 90.f;
	i_currency = 1;
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