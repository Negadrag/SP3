#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"

class Scene
{
public:
	Scene()
	{
		EntityManager::GetInstance()->m_currentSceneID = ++EntityManager::GetInstance()->m_numScene;
		this->m_sceneID = EntityManager::GetInstance()->m_numScene;
		
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