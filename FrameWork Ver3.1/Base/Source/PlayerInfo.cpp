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
	i_currency = 30;
}