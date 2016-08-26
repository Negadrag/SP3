#ifndef MENUCURSOR_H
#define MENUCURSOR_H

#include "Cursor.h"
#include "GameplayCam.h"
#include "GUI.h"
#include "RenderManager.h"


class MenuCursor : public Cursor
{
public:

	MenuCursor();
	~MenuCursor();

	void Init(GameplayCam *camera);
	void Update(const double &dt);

	void SceneMoveNorth(float cam_spd, double dt);
	void SceneMoveRight(float cam_spd, double dt);
	void SceneMoveLeft(float cam_spd, double dt);
	void SceneMoveBack(float cam_spd, double dt);	//Back returns to any previous scene

	//void MovementManager();

	void ButtonManager();
	void MainButtonsInit();
	void InstructionsInit();
	void OptionsInit();

	void MainButtonRender();
	void InstructionsRender();
	void OptionsRender();

	
	void ButtonDeactivator();

	GameplayCam *camera;
	
	bool istransition;

private:
	bool bLButtonState;
	void Clicking(double dt);
	void HotKeys();

	bool movenorth;
	bool moveleft;
	bool moveright;

	GUI* btn_play;				//  play	= 0
	GUI* btn_editor;			//	editor	= 1
	GUI* btn_instructions;		//	instr	= 2
	GUI* btn_option;			//	options	= 3
	GUI* btn_exit;				//	exit	= 4

	GUI* instr_txt;

	GUI* option_1;
	GUI* option_2;
	GUI* option_3;
	GUI* option_4;

	GUI* option_5;
	GUI* option_6;
	GUI* option_7;
	GUI* option_8;

	int menu_states;
	int scene_change;
};

#endif