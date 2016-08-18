#include "EntityManager.h"
#include "Entity.h"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
{
	m_currentSceneID = 0;
	m_numScene = 0;
	list<Entity*> temp;
	entityList.push_back(temp);
}

EntityManager::~EntityManager()
{
}

EntityManager* EntityManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new EntityManager();
	}
	return instance;
}

void EntityManager::AddEntity(Entity* entity)
{
	entity->m_sceneID = this->m_currentSceneID;
	if (m_currentSceneID >= entityList.size())
	{
		list<Entity*> newList;
		//newList.push_back(entity);
		this->entityList.push_back(newList);
		AddEntity(entity);
	}
	this->entityList[m_currentSceneID].push_back(entity);
	
}

void EntityManager::RemoveEntity(Entity* entity)
{
	this->entityList[entity->m_sceneID].remove(entity);
}

void EntityManager::UpdateAllEntity(double dt, int sceneID)
{
	if (sceneID == 0)
	{
		sceneID = this->m_currentSceneID;
	}
	for (list<Entity*>::iterator it = entityList[sceneID].begin(); it != entityList[sceneID].end(); ++it)
	{
			if ((*it)->b_isActive == true)
			{
				(*it)->Update(dt);
			}
	}
}