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
	i_health = 30;
	i_currency = 1000;
	i_essenceBasic = 1000;
	i_essenceIce = 1000;
	i_essenceSpeed = 1000;
	i_essenceTanky = 1000;
	i_showcaseIndex = 0;
	enemyToShowcase.clear();
	encounteredEnemies.clear();
	b_showcaseEnemy = true;
}