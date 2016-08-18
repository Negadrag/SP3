#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <list>
#include <vector>

using std::list;
using std::vector;
class Entity;

class EntityManager
{
public:
	~EntityManager();
	static EntityManager* GetInstance();
	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);
	
	void UpdateAllEntity(double dt, int sceneID = 0);

	vector<list<Entity*>> entityList;
	int m_currentSceneID;
	int m_numScene;
private:
	static EntityManager* instance;


protected:
	EntityManager();
};

#endif