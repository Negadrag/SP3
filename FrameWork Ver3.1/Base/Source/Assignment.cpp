#include "Assignment.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"
#include "Music.h"
#include "BuffTower.h"


Assignment::Assignment():Scene()
{
}

Assignment::~Assignment()
{
}

void Assignment::Init()
{
	this->Init2();
	testMap.LoadMap(std::fstream("Maps//Level-Easy.csv"));
	//this->m_sceneID = 1;

	testMap.waves.player = &(this->player);

	camera.Init(Vector3((float)(testMap.i_columns - 1) / 2.f, (float)testMap.i_rows / 2.f, 10.f), Vector3((float)(testMap.i_columns - 1) / 2.f, (float)testMap.i_rows / 2.f, 0.f), Vector3(0, 1, 0), 30.f);

	player.Init();

	//camera.Init(Vector3(0,-5,10), Vector3(0,0,0), Vector3(0, 1, 0));
	camera.b_ortho = true;
	camera.orthoSize = ((float)testMap.i_rows / 2.f) + 1.f;
	camera.defaultOrtho = camera.orthoSize;
	camera.aspectRatio.Set(4, 3);
	RenderManager::GetInstance()->SetCamera(&camera);

	grass.b_shadows = false;
	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set((float)(testMap.i_columns - 1) / 2.f, (float)testMap.i_rows / 2.f, 0);
	grass.scale.Set(camera.orthoSize * (camera.aspectRatio.x / camera.aspectRatio.y) * 2, camera.orthoSize * 2.5, 1);
	grass.rotation.Set(0, 0, 0);

	cursor.Init(&towerList,testMap.waves.GetEnemyList());
}

void Assignment::Init2()
{
	//Music::GetInstance()->PlayMusic(6, true, 0.2f);
}

void Assignment::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*if (Application::IsKeyPressed('9'))
	{
		ATower.upgrade = true;
	}*/

	if (Application::IsKeyPressed('N'))
	{
		//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		SceneManager::GetInstance()->ChangeScene(8, true);
	}

	testMap.waves.Update(dt);
	fps = (float)(1.f / dt);

	cursor.Update(camera, testMap, dt);
	camera.Update(dt);

	if (player.i_health <= 0)
	{
		SceneManager::GetInstance()->ChangeScene(6,false);
	}

	RenderManager::GetInstance()->SetLight(Vector3(-0.5, -0.5, 1));

	RenderManager::GetInstance()->SetCamera(&camera);
}

void Assignment::Render()
{
	RenderManager::GetInstance()->RenderMesh(GEO_CONE, Vector3(cursor.checkPositionX, cursor.checkPositionY, 0), Vector3(1.f, 1.5f, 1.f), Vector3(90, 0, 0), false, false);

	for (int i = 0; i < testMap.i_rows; ++i) // y - axis
	{
		for (int j = 0; j < testMap.i_columns; ++j) // x - axis
		{
			if (testMap.screenMap[j][i] == -2)
			{
				RenderManager::GetInstance()->RenderMesh(GEO_CUBE2, Vector3(j * testMap.i_tileSize, i  * testMap.i_tileSize, 0), Vector3(1, 1, 1), Vector3(0, -90, 0), true, false);
			}
			else if (testMap.screenMap[j][i] == -1)
			{
				RenderManager::GetInstance()->RenderMesh(GEO_PATH, Vector3(j * testMap.i_tileSize, i  * testMap.i_tileSize, 0.1), Vector3(1, 1, 1), Vector3(0, 0, 0), true, false);
			}
			else if (testMap.screenMap[j][i] == 0 || testMap.screenMap[j][i] == -3)
			{
				RenderManager::GetInstance()->RenderMesh(GEO_GRASS, Vector3(j * testMap.i_tileSize, i  * testMap.i_tileSize, 0.1), Vector3(1, 1, 1), Vector3(0, 0, 0), true, false);
			}
		}
	}

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 21);
	
	ss.str("");
	ss.precision(5);
	ss << "Health left: " << player.i_health;
	RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss.precision(5);
	ss << "Currency: " << player.i_currency;
	RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss.precision(5);
	ss << "ice: " << player.i_essenceIce;
	RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 9);
	ss.str("");
	ss.precision(5);
	ss << "basic: " << player.i_essenceBasic;
	RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 12);
	ss.str("");
	ss.precision(5);
	ss << "tank: " << player.i_essenceTanky;
	RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 15);
	ss.str("");
	ss.precision(5);
	ss << "Speed: " << player.i_essenceSpeed;
	RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 18);
}

void Assignment::Exit()
{
	//clean Up scene Variables
	for (vector<Tower*>::iterator it = towerList.begin(); it != towerList.end(); ++it)
	{
		if (*it != nullptr)
		{
			BuffTower* temp = dynamic_cast<BuffTower*>(*it);
			if (temp)
			{
				temp->towerList = nullptr;
			}
			delete *it;
		}
	}
	towerList.clear();
	cursor.Clear();
	testMap.Exit();
}