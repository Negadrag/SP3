#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	Init();
}

PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::Init()
{
	tempCamera = nullptr;
	i_health = 30;
	i_currency = 3000;
	i_essenceBasic = 1000;
	i_essenceIce = 1000;
	i_essenceSpeed = 0;
	i_essenceTanky = 0;
	i_showcaseIndex = 0;
	enemyToShowcase.clear();
	encounteredEnemies.clear();
	b_showcaseEnemy = true;
}