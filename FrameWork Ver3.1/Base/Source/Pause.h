#ifndef PAUSE_H
#define PAUSE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "TurntableCam.h"
#include "TileMap.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Terrain.h"
#include "OrthoCamera.h"
#include "WaveManager.h"
#include "ArrowTower.h"
#include "CannonTower.h"
#include "PauseCursor.h"

using std::vector;

class Pause : public Scene
{
	
public:
	Pause();
	~Pause();

	virtual void Init();
	virtual void Init2();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	int m_previousSceneID;

	Renderable hi;

	PauseCursor cursor;
private:
	OrthoCamera camera;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;

};

#endif