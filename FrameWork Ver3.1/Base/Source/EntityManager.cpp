#include "EntityManager.h"
#include "Entity.h"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
{
	m_currentSceneID = 0;
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
	this->entityList.push_back(entity);
	
}

void EntityManager::RemoveEntity(Entity* entity)
{
	this->entityList.remove(entity);
}

void EntityManager::UpdateAllEntity(double dt, int sceneID)
{
	if (sceneID == 0)
	{
		sceneID = this->m_currentSceneID;
	}
	for (list<Entity*>::iterator it = entityList.begin(); it != entityList.end(); ++it)
	{
		if ((*it)->m_sceneID == sceneID)
		{
			if ((*it)->b_isActive == true)
			{
				(*it)->Update(dt);
			}
		}

	}
}