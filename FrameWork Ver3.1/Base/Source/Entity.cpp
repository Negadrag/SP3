#include "Entity.h"
#include "EntityManager.h"

Entity::Entity()
{
	EntityManager::GetInstance()->AddEntity(this);
	pos.Set(0, 0, 0);
	rotation.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	b_isActive = true;
}


Entity::~Entity()
{

}

//init function to init all variables
void Entity::Init(int sceneID,Vector3 pos, Vector3 scale, Vector3 rotation)
{
	this->pos = pos;
	this->scale = scale;
	this->rotation = rotation;
	this->m_sceneID = sceneID;
}

//getters and setters
//Vector3 Entity::GetPos()
//{
//	return this->pos;
//}
//
//Vector3 Entity::GetScale()
//{
//	return this->scale;
//}
//
//Vector3 Entity::GetRotation()
//{
//	return this->rotation;
//}
//
//bool Entity::GetIsActive()
//{
//	return this->b_isActive;
//}
//
//void Entity::SetPos(Vector3 pos)
//{
//	this->pos = pos;
//}
//
//void Entity::SetScale(Vector3 scale)
//{
//	this->scale = scale;
//}
//
//void Entity::SetRotation(Vector3 rotation)
//{
//	this->rotation = rotation;
//}
//
//void Entity::SetActive(bool isActive)
//{
//	this->b_isActive = isActive;
//}

//default update
void Entity::Update(double dt)
{
}