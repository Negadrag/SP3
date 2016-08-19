#ifndef CAPTURE_GAME_H
#define CAPTURE_GAME_H

#include "Scene.h"
#include "Mtx44.h"
#include "GameplayCam.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Terrain.h"
#include "Application.h"
#include "GameObject.h"
#include "ParticleGenerator.h"

using std::vector;

class CaptureGame : public Scene
{

public:
	CaptureGame();
	~CaptureGame();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	bool CheckCollision(GameObject* go, GameObject* other, double dt);
	float CheckCollision2(GameObject* go, GameObject* other);
	void CollisionResponse(GameObject*go, GameObject*other);
	//void RenderGO(GameObject *go);
	void CreateScene();
	void ClearScene();

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

private:
	GameplayCam camera;

	float windmillRotateAngle;

	Renderable grass;

	//Capture Game Objects
	GameObject* ren_ball;
	GameObject* ren_wall;
	GameObject* ren_pillar;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;
	bool b_initScene;
	Terrain terrain;
	ParticleGenerator whitebang;
	ParticleGenerator bluebang;
	ParticleGenerator redbang;
	ParticleGenerator yellowbang;
	ParticleGenerator greenbang;

protected:
	std::vector<GameObject *> m_goList;
	float m_speed;
	Vector3 m_gravity;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	int m_objectCount;
	float EstimatedTime, TimeTaken;
	bool timerStarted;

	float m1, m2;
	Vector3 u1, u2, v1, v2;

	bool isrunning;
	bool b_allBallsdespawned;
	float f_ballSpawnTimer;

	int balls;

	int resource1;
	int resource2;
	int resource3;
	int resource4;

	int bonuscount;


};

#endif