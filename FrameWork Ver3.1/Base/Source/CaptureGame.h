#ifndef CAPTURE_GAME_H
#define CAPTURE_GAME_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Terrain.h"
#include "Application.h"

using std::vector;

class CaptureGame : public Scene
{

public:
	CaptureGame(int sceneID = 0);
	~CaptureGame();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();



private:
	Camera3 camera;

	float windmillRotateAngle;

	Renderable grass;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;

	Terrain terrain;
};

#endif