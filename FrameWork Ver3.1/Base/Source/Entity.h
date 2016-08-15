#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"

class Entity
{
public:
	
	virtual ~Entity();
	
	virtual void Init(int sceneID,Vector3,Vector3,Vector3);
	virtual void Update(double dt);

	Vector3 pos;
	Vector3 scale;
	Vector3 rotation;
	bool b_isActive;
	int m_sceneID;

	//Getters and setters
	//Vector3 GetPos();
	//Vector3 GetScale();
	//Vector3 GetRotation();
	//bool GetIsActive();
	//void SetPos(Vector3 pos);
	//void SetScale(Vector3 scale);
	//void SetRotation(Vector3 rotation);
	//void SetActive(bool isActive);
	//
private:
	
protected:
	Entity();

};

#endif