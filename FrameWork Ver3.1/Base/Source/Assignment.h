#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

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

using std::vector;

class Assignment : public Scene
{
	
public:
	Assignment(int sceneID=0);
	~Assignment();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();



private:
	//Camera3 camera;
	//TurntableCam turntab;
	OrthoCamera camera;

	float windmillRotateAngle;

	Renderable grass;
	Renderable testball;

	TileMap testMap;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;

	Terrain terrain;
};

#endif