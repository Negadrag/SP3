#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"
#include "PlayerInfo.h"

class Scene
{
public:
	Scene();
	~Scene();

	int m_sceneID;
	bool b_frozen;
	bool b_changeScene;
	virtual void Init() = 0;
	virtual void Init2(){}; // variables to initialise when coming back into a frozen scene.
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	static PlayerInfo player;
private:

protected:

};

#endif