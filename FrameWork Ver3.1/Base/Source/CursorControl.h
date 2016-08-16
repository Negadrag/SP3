#ifndef CURSORCONTROL_H
#define CURSORCONTROL_H

#include "Camera.h"
#include "TileMap.h"
#include "Tower.h"

class CursorControl
{
public:
	CursorControl();
	~CursorControl();
	void Update(const Camera &camera, const TileMap &tileMap);
	int checkPositionX, checkPositionY;

};

#endif