#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <vector>
#include <list>
#include "RenderManager.h"

using std::vector;
using std::list;

class GUI;

class GUIManager
{
public:
	~GUIManager();
	vector<list<GUI*>> GUIList;
	int m_currentSceneID;

	void AddGUI(GUI* gui);
	void RemoveGUI(GUI* gui);

	void RenderAllGUI();

	GUI* FindGUI(float screenX, float screenY); // Range of -0.5 - 0.5

	static GUIManager* GetInstance();
protected:
	GUIManager();
	static GUIManager* instance;
};

#endif