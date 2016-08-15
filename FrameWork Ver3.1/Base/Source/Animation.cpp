#include "Animation.h"

Animation::Animation()
{
}

void Animation::Set(int startFrame, int endFrame, int repeat, float time, bool active)
{
	this->startFrame = startFrame;
	this->endFrame = endFrame;
	this->repeatCount = repeat;
	this->animTime = time;
	this->animActive = active;
}