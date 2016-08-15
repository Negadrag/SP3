#ifndef ANIMATION_H
#define ANIMATION_H

struct Animation
{
	Animation();

	void Set(int startFrame, int endFrame, int repeat, float time, bool active);

	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
	bool animActive;
};

#endif