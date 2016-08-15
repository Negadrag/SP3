#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <list>

using std::list;
class Entity;

class EntityManager
{
public:
	~EntityManager();
	static EntityManager* GetInstance();
	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);
	
	void UpdateAllEntity(double dt, int sceneID = 0);

	list<Entity*> entityList;
	int m_currentSceneID;
private:
	static EntityManager* instance;


protected:
	EntityManager();
};

#endif