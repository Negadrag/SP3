#ifndef MENUCURSOR_H
#define MENUCURSOR_H

#include "Cursor.h"
#include "GameplayCam.h"
#include "GUI.h"

#include "RenderManager.h"
#include "ParticleGenerator.h"
#include "Music.h"


class MenuCursor : public Cursor
{
public:

	MenuCursor();
	~MenuCursor();

	void Init(GameplayCam *camera);
	void Update(const double &dt);

	void SceneMoveNorth(float cam_spd, double dt);
	void SceneMoveRight(float cam_spd, double dt);
	void SceneMoveDark(float cam_spd, double dt);
	void SceneLevelSelect(float cam_spd, double dt);
	void SceneMoveDown(float cam_spd, double dt);
	void SceneMoveBack(float cam_spd, double dt);	//Back returns to any previous scene

	//void MovementManager();

	void ButtonManager();
	void MainButtonsInit();
	void LvlSelectInit();
	void InstructionsInit();
	void OptionsInit();

	void MainButtonRender();
	void LvlSelectRender();
	void InstructionsRender();
	void OptionsRender();

	void SmokeInit();
	void ButtonDeactivator();

	GameplayCam *camera;
	
	bool istransition;

private:
	bool bLButtonState;
	void Clicking(double dt);
	void HotKeys();

	bool movenorth;
	bool movedark;//black screen
	bool moveright;

	GUI* btn_play;				//  lvlsel	= 0	 -------------->	lvls 1 = 11
	GUI* btn_editor;			//	editor	= 1						lvls 2 = 12
	GUI* btn_instructions;		//	instr	= 2						lvls 3 = 13
	GUI* btn_option;			//	options	= 3	 ->	shadows	= 21	lvls 4 = 14
	GUI* btn_exit;				//	exit	= 4		volume = 22 

	GUI* btn_lvl1;
	GUI* btn_lvl2;
	GUI* btn_lvl3;
	GUI* btn_lvl4;
	GUI* btn_custom;

	GUI* instr_title;

	GUI* option_2;
	GUI* option_6;
	GUI* optionborder;	//bar border
	

	GUI* pressrighttoreturn;
	ParticleGenerator smoke1;
	ParticleGenerator smoke2;
	ParticleGenerator smoke3;

	

	bool holdingdrag;

	/*
	0 = main state
	5 = levelselect
	2 = north (instructions)
	3 = right (options)
	4 = exit
	1 = down ( black screen )*/
	int menu_states;

	float testx, testy, testz;
	float clickoffsetX;

	float adjsizeX;	//size of volume bar and scale
	float adjsizeY;	

	int scene_change;

};

#endif