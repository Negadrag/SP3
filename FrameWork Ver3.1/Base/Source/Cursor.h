#ifndef CURSOR_H
#define CURSOR_H

#include "OrthoCamera.h"
#include "TileMap.h"
#include "MatrixStack.h"
#include "RenderManager.h"

class Cursor
{
public:
	Cursor();
	~Cursor();

	void Update(OrthoCamera &camera, const TileMap &tileMap, const double &dt);
	Vector3 worldCoords;
	
	float worldX, worldY;

protected:
	void CameraBounds(OrthoCamera &camera);
	void EdgePanning(const double &dt, OrthoCamera &camera, float worldX, float worldY, float speed);
};

#endif