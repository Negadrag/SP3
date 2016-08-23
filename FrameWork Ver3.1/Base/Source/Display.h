#ifndef DISPLAY_SCENE_H
#define DISPLAY_SCENE_H

#include "Scene.h"
#include "Mtx44.h"
#include "GameplayCam.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Terrain.h"
#include "Application.h"
#include "GameObject.h"
#include "ParticleGenerator.h"

using std::vector;

class Display : public Scene
{

public:
	Display();
	~Display();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//void RenderGO(GameObject *go);
	void CreateScene();
	void ClearScene();

	void CreateSkybox();


private:
	GameplayCam camera;

	float windmillRotateAngle;

	Renderable grass;
	Renderable demoObject;

	Renderable skytop;
	Renderable skybottom;
	Renderable skyleft;
	Renderable skyright;
	Renderable skyfront;
	Renderable skyback;
	//Renderable forValor;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;
	bool b_initScene;
	//Terrain terrain;
	ParticleGenerator redfall;

	float testx, testy, testz;


};

#endif