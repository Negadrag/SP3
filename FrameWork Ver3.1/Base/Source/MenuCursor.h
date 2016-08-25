#ifndef MENUCURSOR_H
#define MENUCURSOR_H

#include "Cursor.h"
#include "GameplayCam.h"


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

	GameplayCam *camera;

private:
	bool bLButtonState;
	void Clicking(double dt);
	void HotKeys();

	bool istransition;

	int menu_state;
};

#endif