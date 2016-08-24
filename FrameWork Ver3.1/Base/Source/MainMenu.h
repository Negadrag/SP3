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
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//void RenderGO(GameObject *go);
	void CreateScene();
	void ClearScene();

	void SceneDeco();
	void ButtonManager();
	void MainButtons();
	void OptionButtons();

	void CreateSkybox();

	GameplayCam camera;

private:
	

	float windmillRotateAngle;
	float banner_forward;
	float banner_backward;

	Renderable grass;

	/*play, editor, exit takes us out of the scene.
	No spaces will be given within the deco space*/
	Renderable demo_main;
	Renderable demo_play;	//play represents play, edit and exit transition space
	Renderable demo_instr;
	Renderable demo_option;

	


	Renderable skytop;
	Renderable skybottom;
	Renderable skyleft;
	Renderable skyright;
	Renderable skyfront;
	Renderable skyback;
	//Renderable forValor;
	
	
	GUI* btn_play;				//  play	= 0
	GUI* btn_editor;			//	editor	= 1
	GUI* btn_instructions;		//	instr	= 2
	GUI* btn_option;			//	options	= 3
	GUI* btn_exit;				//	exit	= 4

	MenuCursor cursor;

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