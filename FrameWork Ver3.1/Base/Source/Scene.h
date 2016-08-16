#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"

class Scene
{
public:
	Scene(int sceneID = 0)
	{
		EntityManager::GetInstance()->m_currentSceneID = sceneID;
		this->m_sceneID = sceneID;
		
		b_frozen = false; 
		b_changeScene = false;
	}
	~Scene() {}

	int m_sceneID;
	bool b_frozen;
	bool b_changeScene;
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

private:

protected:

};

#endif