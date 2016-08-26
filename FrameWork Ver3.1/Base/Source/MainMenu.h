#ifndef MAINMENU_H
#define MAINMENU_H

#include "Scene.h"
#include "Mtx44.h"
#include "GameplayCam.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Terrain.h"
#include "Application.h"
#include "ParticleGenerator.h"
#include "GUI.h"
#include "MenuCursor.h"

using std::vector;

class MainMenu : public Scene
{

public:


	MainMenu();
	~MainMenu();

	virtual void Init();
	virtual void Init2();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//void RenderGO(GameObject *go);
	void CreateScene();
	void ClearScene();

	void SceneDeco();
	
	void OptionButtons();

	void CreateSkybox();

	GameplayCam camera;

private:
	

	float windmillRotateAngle;
	float banner_forward;
	float banner_backward;

	Renderable grass;

	Renderable ren_menutown;


	Renderable skytop;
	Renderable skybottom;
	Renderable skyleft;
	Renderable skyright;
	Renderable skyfront;
	Renderable skyback;
	//Renderable forValor;
	
	
	

	MenuCursor cursor;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;
	bool b_initScene;
	//Terrain terrain;
	ParticleGenerator redfall;
	ParticleGenerator waterfountain;

	float testx, testy, testz;


};

#endif