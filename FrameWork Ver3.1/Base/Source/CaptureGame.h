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

	int resource1;
	int resource2;
	int resource3;
	int resource4;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//void RenderGO(GameObject *go);
	void CreateScene();
	void ClearScene();

	void SwitchInitializer();
	//void SwitchInitializer(bool blue, bool red, bool yellow, bool green);
	//Generates patterns of the minigame scene
	void CreateTypeONE(int type);
	void CreateTypeTWO(int type1, int type2);
	void CreateTypeTHREE(int type1, int type2, int type3);
	void CreateTypeFOUR();


	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

private:
	GameplayCam camera;

	float windmillRotateAngle;

	Renderable grass;
	Renderable pot;
	//Renderable forValor;

	bool bLightEnabled;
	Vector3 distance;
	float angle;
	float fps;
	bool b_initScene;
	//Terrain terrain;
	ParticleGenerator whitebang;
	ParticleGenerator bluebang;
	ParticleGenerator redbang;
	ParticleGenerator yellowbang;
	ParticleGenerator greenbang;

	float f_ballSpawnDebounceTimer;

protected:
	std::vector<GameObject *> m_goList;
	float m_speed;
	Vector3 m_gravity;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	float EstimatedTime, TimeTaken;
	bool timerStarted;
	
	bool isrunning;
	bool b_allBallsdespawned;
	bool b_showBanner;
	float f_ballSpawnTimer;
	float bannerpos;
	//25, 50

	int balls;
	int bonuscount;

	int top_rarest; //blue,red,yelo,green = 1,2,3,4

	bool blueswitch;
	bool redswitch;
	bool yellowswitch;
	bool greenswitch;

	/* counts the number of active resource thats true, 
	patterns of walls is based on number of bools switched on*/
	int total_active_switches;

};

#endif