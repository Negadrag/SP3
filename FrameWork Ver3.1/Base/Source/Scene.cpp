#include "Scene.h"

PlayerInfo Scene::player;

Scene::Scene()
{
	EntityManager::GetInstance()->m_currentSceneID = ++EntityManager::GetInstance()->m_numScene;
	this->m_sceneID = EntityManager::GetInstance()->m_numScene;

	b_frozen = false;
	b_changeScene = false;
}

Scene::~Scene()
{
}