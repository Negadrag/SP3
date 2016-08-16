#ifndef CURSORCONTROL_H
#define CURSORCONTROL_H

#include "Camera.h"
#include "TileMap.h"

class CursorControl
{
public:
	CursorControl();
	~CursorControl();
	void Update(const Camera &camera, const TileMap &tileMap);
	int x, y;
};

#endif