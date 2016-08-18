#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Terrain.h"

using std::vector;

class TestScene : public Scene
{

public:
	TestScene();
	~TestScene();

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