#ifndef LOSE_SCENE_H
#define LOSE_SCENE_H

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

class LoseScene : public Scene
{

public:
	LoseScene();
	~LoseScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//void RenderGO(GameObject *go);
	void CreateScene();
	void ClearScene();


private:
	GameplayCam camera;

	float windmillRotateAngle;

	Renderable grass;
	//Renderable forValor;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;
	bool b_initScene;
	//Terrain terrain;
	ParticleGenerator redfall;


};

#endif