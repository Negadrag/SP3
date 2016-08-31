#include "CaptureGame.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"

CaptureGame::CaptureGame() :Scene()
{
}

CaptureGame::~CaptureGame()
{
}

void CaptureGame::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	m_speed = 1.f;
	m_gravity.Set(0, -10, 0);
	Math::InitRNG();

	m_objectCount = 0;
	resource1 = 0;
	resource2 = 0;
	resource3 = 0;
	resource4 = 0;

	balls = 20;
	bonuscount = 0;

	top_rarest = 0;
	total_active_switches = 0; 

	blueswitch = false;
	redswitch = false; 
	yellowswitch = false;
	greenswitch = false;

	isrunning = false;
	b_allBallsdespawned = false;
	f_ballSpawnDebounceTimer = 0.f;

	//SwitchInitializer();

	CreateScene();
	camera.Init(Vector3(0, 0, 1500), Vector3(0, 0, 0), Vector3(0, 1, 0),0);
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	f_ballSpawnTimer = 0.f;
	std::cout << "entered";

}

void CaptureGame::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//if (b_initScene == false)
	//{
	//	
	//	b_initScene = true;
	//}
	if (Application::IsKeyPressed('U'))
	{
		balls = 30;
		b_allBallsdespawned = false;
	}
	

	fps = (float)(1.f / dt);

	f_ballSpawnDebounceTimer += dt;

	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);
	if (Application::IsKeyPressed('B'))
	{
		std::cout << "hi" << std::endl;
	}

	static bool bLButtonState = false;
	//if (!bLButtonState && Application::IsMousePressed(0))
	if (Application::IsMousePressed(0))
	{
		//bLButtonState = true;
		//std::cout << "down" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();

		x = (x / (w)) - 0.5f;
		y = 800;

		float worldX = x * camera.orthoSize * 2 * (camera.aspectRatio.x/camera.aspectRatio.y);
		worldX = Math::Clamp(worldX, -450.f, 450.f);
		float worldY = y;
		
		if (balls > 0 && f_ballSpawnDebounceTimer >=1.f/4.f)
		{
			f_ballSpawnDebounceTimer = 0.f;
			GameObject * ball = FetchGO(GameObject::GO_BALL);
			//ball->type = GameObject::GO_BALL;
			//ball->pos.Set(0, 0, 0);// = m_ghost->pos;
			ball->pos.Set(worldX, worldY, 0);
			ball->vel.Set(0, 0, 0);
			ball->scale.Set(50, 50, 50);
			ball->mass = 1.f;
			balls--;
		}


	}
	//else if (bLButtonState && !Application::IsMousePressed(0))
	else if(!Application::IsMousePressed(0))
	{
		//bLButtonState = false;
		//std::cout << "up" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		x = m_worldWidth * (x / w);
		y = m_worldHeight * ((h - y) / h);

//		bonuscount = 0;

	


		EstimatedTime = -1;
		TimeTaken = 0;
		timerStarted = true;

	}
	
	whitebang.Update(dt);
	bluebang.Update(dt);
	redbang.Update(dt);
	yellowbang.Update(dt);
	greenbang.Update(dt);



	b_allBallsdespawned = true;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end();)
	{
		if ((*it)->type == GameObject::GO_BALL && (*it)->b_isActive == true)
		{
			b_allBallsdespawned = false;
		}
		GameObject *go = (GameObject *)*it;
		if (go->b_isActive)
		{
			if (go->type == GameObject::GO_BALL)
			{
				go->vel += m_gravity;
				go->pos += go->vel * dt * m_speed;
				go->vel *= 0.98;

				float radius = go->scale.x;

				if ((go->pos.y > 1000) ||
					(go->pos.y < -600))
				{
					if (go->pos.x >= -100 && go->pos.x <= 100)
					{
						bonuscount++;

						whitebang.SetType(GEO_PARTICLE_WHITE);
						//whitebang.pos.Set(go->pos.x, go->pos.y, go->pos.z);
						whitebang.SetFrequency(1);
						whitebang.SetCap(1000);
						whitebang.i_spawnAmount = 30;
						whitebang.f_lifeTime = 15.f;
						whitebang.minVel.Set(-10, 5, 0);
						whitebang.maxVel.Set(10, 100, 0);
						whitebang.scale.Set(20, 20, 20);
						whitebang.f_maxDist = 100.f;
						whitebang.isActive = false;
						whitebang.SpawnParticle(Vector3(go->pos.x, go->pos.y, 10));
					}

					go->b_isActive = false;
					delete go;
					it = m_goList.erase(it);
					m_objectCount--;
					continue;
				}

				if (go->pos.z >= 5)
				{
					go->b_isActive = false;
					delete go;
					it = m_goList.erase(it);
					m_objectCount--;
					continue;
				}
				if (go->pos.z <= -5)
				{
					go->b_isActive = false;
					delete go;
					it = m_goList.erase(it);
					m_objectCount--;
					continue;
				}
			}

			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject*other = (GameObject*)*it2;
				if (!other->b_isActive) continue;

				go->HandleCollision(other,dt);
			}
			++it;
		}

	}
	if (balls > 0)
	{
		b_allBallsdespawned = false;
	}

	if (b_allBallsdespawned == true)
	{
		if (bonuscount == 0)
		{
			player.i_essenceBasic += resource4;
			player.i_essenceIce += resource1;
			player.i_essenceSpeed += resource3;
			player.i_essenceTanky += resource2;
			SceneManager::GetInstance()->ChangeScene(player.m_sceneID, false);
		}
		balls = bonuscount;
		bonuscount = 0;
		b_allBallsdespawned = false;

	}

	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(1, false);
	}
	if (Application::IsKeyPressed(VK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene(5, false);
	}

}

void CaptureGame::Render()
{
	int spacing = 13;

	RenderManager::GetInstance()->RenderTextOnScreen("No. of Balls: ", Color(1, 1, 1), 2, 0, 40);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(balls), Color(1, 1, 1), 2, spacing, 40);


	RenderManager::GetInstance()->RenderTextOnScreen("Blue Items: ", Color(0, 0.5, 1), 2, 0, 35);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource1), Color(0, 0.5, 1), 2, spacing, 35);

	RenderManager::GetInstance()->RenderTextOnScreen("Red Items: ", Color(1, 0, 0), 2, 0, 20);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource2), Color(1, 0, 0), 2, spacing, 20);

	RenderManager::GetInstance()->RenderTextOnScreen("YellowItems: ", Color(1, 1, 0), 2, 0, 30);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource3), Color(1, 1, 0), 2, spacing, 30);

	RenderManager::GetInstance()->RenderTextOnScreen("Green Items: ", Color(0, 0.7, 0), 2, 0, 25);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource4), Color(0, 0.7, 0), 2, spacing, 25);

	RenderManager::GetInstance()->RenderTextOnScreen("FPS: ", Color(1, 1, 1), 2, 0, 15);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(fps), Color(1, 1, 1), 2, spacing, 15);


	RenderManager::GetInstance()->RenderTextOnScreen("BonusCount: ", Color(1, 1, 1), 2, 0, 45);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(bonuscount), Color(1, 1, 1), 2, spacing, 45);

}

GameObject* CaptureGame::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	GameObject* go;
	GameObject* go2;
	if (type == GameObject::GO_BALL)
	{
		go = new GameObject();
		go->type = GameObject::GO_BALL;
		go->meshID = GEO_SPHERE;
		go->pos.Set(0, 0, 0);
		go->scale.Set(1, 1, 1);
		go->vel.Set(0, 0, 0);
		go->b_isActive = true;
		m_goList.push_back(go);
		m_objectCount++;

	}
	if (type == GameObject::GO_WALL)
	{
		go = new GameObject();
		go->type = GameObject::GO_WALL;
		go->meshID = GEO_CUBE;
		go->pos.Set(0, 0, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		//go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}

	if (type == GameObject::GO_RESOURCE1)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE1;
		go->meshID = GEO_BLUECUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		go->score = &resource1;
		go->particleGenerator = &bluebang;
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_RESOURCE2)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE2;
		go->meshID = GEO_REDCUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		go->score = &resource2;
		go->particleGenerator = &redbang;
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_RESOURCE3)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE3;
		go->meshID = GEO_YELLOWCUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		go->score = &resource3;
		go->particleGenerator = &yellowbang;
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_RESOURCE4)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE4;
		go->meshID = GEO_GREENCUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		go->score = &resource4;
		go->particleGenerator = &greenbang;
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_BONUSPAD)
	{
		go = new GameObject();
		go->type = GameObject::GO_BONUSPAD;
		go->meshID = GEO_CUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}

	return go;
}

void CaptureGame::SwitchInitializer()
{
	//check for total switches turned on
	if (blueswitch)
		total_active_switches++;
	if (redswitch)
		total_active_switches++;
	if (yellowswitch)
		total_active_switches++;
	if (greenswitch)
		total_active_switches++;

	//check for highest rarity
	if (greenswitch)
		top_rarest = 4;
	else if (redswitch)
		top_rarest = 2;
	else if (yellowswitch)
		top_rarest = 3;
	else
		top_rarest = 1;		//blue (common)
	


	//FKING Cancerous CODE!!
	if (total_active_switches == 4)
	{
		CreateTypeFOUR();
	}
	else if (total_active_switches == 3)
	{
		int temp1 = 0;
		int temp2 = 0;
		int temp3 = 0;


		if (greenswitch)				//checking for green first
		{
			temp1 = 4;
			if (blueswitch)				//if blueswitch
			{								//check for red 
				temp2 = 1;					//then yellow
				if (redswitch)				
					temp3 = 2;				
				else
					temp3 = 3;
			}

			else if (redswitch)			//if red
			{								//check for blue
				temp2 = 2;					//then yellow
				if (blueswitch)
					temp3 = 1;
				else
					temp3 = 3;
			}
			else if (yellowswitch)		//if yellow
			{								//check for red 
				temp2 = 3;					//then blue
				if (redswitch)
					temp3 = 2;
				else
					temp3 = 1;
			}
		}


		else if (redswitch)			
		{								//checking for red first
			temp1 = 2;
			if (blueswitch)
			{							//if blueswitch
				temp2 = 1;					//check for green 
				if (greenswitch)			//then yellow
					temp3 = 4;
				else
					temp3 = 3;
			}

			else if (greenswitch)
			{							//if green
				temp2 = 4;					//check for blue
				if (blueswitch)				//then yellow
					temp3 = 1;
				else
					temp3 = 3;
			}
			else if (yellowswitch)
			{							//if yellow
				temp2 = 3;					//check for green 
				if (greenswitch)				//then blue
					temp3 = 4;
				else
					temp3 = 1;
			}
		}
		else if (yellowswitch)
		{								//checking for yellow first
			temp1 = 3;
			if (blueswitch)
			{							//if blueswitch
				temp2 = 1;					//check for green 
				if (greenswitch)			//then red
					temp3 = 4;
				else
					temp3 = 2;
			}

			else if (greenswitch)
			{							//if green
				temp2 = 4;					//check for blue
				if (blueswitch)				//then red
					temp3 = 1;
				else
					temp3 = 2;
			}
			else if (redswitch)
			{							//if red
				temp2 = 2;					//check for green 
				if (greenswitch)				//then blue
					temp3 = 4;
				else
					temp3 = 1;
			}
		}
		else
		{								//checking for blue first
			temp1 = 1;
			if (yellowswitch)
			{							//if yellow
				temp2 = 3;					//check for green 
				if (greenswitch)			//then red
					temp3 = 4;
				else
					temp3 = 2;
			}

			else if (greenswitch)
			{							//if green
				temp2 = 4;					//check for yellow
				if (yellowswitch)				//then red
					temp3 = 3;
				else
					temp3 = 2;
			}
			else if (redswitch)
			{							//if red
				temp2 = 2;					//check for green 
				if (greenswitch)				//then yellow
					temp3 = 4;
				else
					temp3 = 3;
			}
		}


		CreateTypeTHREE(temp1, temp2, temp3);
	}


	else if (total_active_switches == 2)
	{
		int temp1 = 0;
		int temp2 = 0;


		if (greenswitch)				//checking for green first
		{
			temp1 = 4;
			if (blueswitch)				//if blueswitch
				temp2 = 1;					
			else if (redswitch)			//if red
				temp2 = 2;					
			else if (yellowswitch)		//if yellow
				temp2 = 3;		
		}
		else if (redswitch)
		{								//checking for red first
			temp1 = 2;
			if (blueswitch)				//if blueswitch
				temp2 = 1;			
			else if (greenswitch)		//if green
				temp2 = 4;				
			else if (yellowswitch)		//if yellow
				temp2 = 3;				
		}
		else if (yellowswitch)
		{								//checking for yellow first
			temp1 = 3;
			if (blueswitch)				//if blueswitch
				temp2 = 1;			
			else if (greenswitch)		//if green
				temp2 = 4;	
			else if (redswitch)			//if red
				temp2 = 2;	
		}
		else
		{								//checking for blue first
			temp1 = 1;
			if (yellowswitch)			//if yellow
				temp2 = 3;
			else if (greenswitch)		//if green
				temp2 = 4;
			else if (redswitch)
				temp2 = 2;				//if red	
		}


		CreateTypeTWO(temp1, temp2);
	}


	else if (total_active_switches == 1)
	{
		int temp1 = 0;

		if (greenswitch)				//checking for green 
		{
			temp1 = 4;
		}
		else if (redswitch)
		{								//checking for red 
			temp1 = 2;
		}
		else if (yellowswitch)
		{								//checking for yellow 
			temp1 = 3;
		}
		else
		{								//checking for blue 
			temp1 = 1;
		}

		CreateTypeONE(temp1);
	}
	
	std::cout << top_rarest << std::endl;
}

void CaptureGame::CreateTypeONE(int type)
{
	Mtx44 rotate;

	GameObject::GAMEOBJECT_TYPE passedWall;

	if (type == 1)
		passedWall = GameObject::GO_RESOURCE1;
	if (type == 2)
		passedWall = GameObject::GO_RESOURCE2;
	if (type == 3)
		passedWall = GameObject::GO_RESOURCE3;
	if (type == 4)
		passedWall = GameObject::GO_RESOURCE4;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			GameObject* wall = FetchGO(passedWall);
			wall->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
			wall->scale.Set(30, 30, 15);
			wall->rotation.Set(0, 0, 55);
			rotate.SetToIdentity();
			rotate.SetToRotation(-55, 0, 0, 1);
			wall->normal = rotate* wall->normal;
			wall->b_lightEnabled = false;
			

			GameObject* wall2 = FetchGO(passedWall);
			wall2->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
			wall2->scale.Set(30, 30, 15);
			wall2->rotation.Set(0, 0, -55);
			rotate.SetToIdentity();
			rotate.SetToRotation(55, 0, 0, 1);
			wall2->normal = rotate* wall2->normal;
			wall2->b_lightEnabled = false;
		

		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{

			GameObject* wall = FetchGO(passedWall);
			wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
			wall->scale.Set(30, 30, 15);
			wall->rotation.Set(0, 0, 55);
			rotate.SetToIdentity();
			rotate.SetToRotation(-55, 0, 0, 1);
			wall->normal = rotate* wall->normal;
			wall->b_lightEnabled = false;
		

			GameObject* wall2 = FetchGO(passedWall);
			wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
			wall2->scale.Set(30, 30, 15);
			wall2->rotation.Set(0, 0, -55);
			rotate.SetToIdentity();
			rotate.SetToRotation(55, 0, 0, 1);
			wall2->normal = rotate* wall2->normal;
			wall2->b_lightEnabled = false;
			

		}
	}
}

void CaptureGame::CreateTypeTWO(int type1, int type2)
{
	Mtx44 rotate;

	GameObject::GAMEOBJECT_TYPE passedWall_1;
	GameObject::GAMEOBJECT_TYPE passedWall_2;

	//passedWall_1 is the higher rarity
	if (top_rarest == type1)
	{
		if (type1 == 1)
			passedWall_1 = GameObject::GO_RESOURCE1;
		if (type1 == 2)
			passedWall_1 = GameObject::GO_RESOURCE2;
		if (type1 == 3)
			passedWall_1 = GameObject::GO_RESOURCE3;
		if (type1 == 4)
			passedWall_1 = GameObject::GO_RESOURCE4;

		if (type2 == 1)
			passedWall_2 = GameObject::GO_RESOURCE1;
		if (type2 == 2)
			passedWall_2 = GameObject::GO_RESOURCE2;
		if (type2 == 3)
			passedWall_2 = GameObject::GO_RESOURCE3;
		if (type2 == 4)
			passedWall_2 = GameObject::GO_RESOURCE4;
	}
	else
	{
		if (type2 == 1)
			passedWall_1 = GameObject::GO_RESOURCE1;
		if (type2 == 2)
			passedWall_1 = GameObject::GO_RESOURCE2;
		if (type2 == 3)
			passedWall_1 = GameObject::GO_RESOURCE3;
		if (type2 == 4)
			passedWall_1 = GameObject::GO_RESOURCE4;

		if (type1 == 1)
			passedWall_2 = GameObject::GO_RESOURCE1;
		if (type1 == 2)
			passedWall_2 = GameObject::GO_RESOURCE2;
		if (type1 == 3)
			passedWall_2 = GameObject::GO_RESOURCE3;
		if (type1 == 4)
			passedWall_2 = GameObject::GO_RESOURCE4;
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			GameObject* wall = FetchGO(passedWall_2);
			wall->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
			wall->scale.Set(30, 30, 15);
			wall->rotation.Set(0, 0, 55);
			rotate.SetToIdentity();
			rotate.SetToRotation(-55, 0, 0, 1);
			wall->normal = rotate* wall->normal;
			wall->b_lightEnabled = false;
			

			GameObject* wall2 = FetchGO(passedWall_2);
			wall2->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
			wall2->scale.Set(30, 30, 15);
			wall2->rotation.Set(0, 0, -55);
			rotate.SetToIdentity();
			rotate.SetToRotation(55, 0, 0, 1);
			wall2->normal = rotate* wall2->normal;
			wall2->b_lightEnabled = false;
		

		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{

			GameObject* wall = FetchGO(passedWall_1);
			wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
			wall->scale.Set(30, 30, 15);
			wall->rotation.Set(0, 0, 55);
			rotate.SetToIdentity();
			rotate.SetToRotation(-55, 0, 0, 1);
			wall->normal = rotate* wall->normal;
			wall->b_lightEnabled = false;
			

			GameObject* wall2 = FetchGO(passedWall_1);
			wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
			wall2->scale.Set(30, 30, 15);
			wall2->rotation.Set(0, 0, -55);
			rotate.SetToIdentity();
			rotate.SetToRotation(55, 0, 0, 1);
			wall2->normal = rotate* wall2->normal;
			wall2->b_lightEnabled = false;
			
		}
	}
}

void CaptureGame::CreateTypeTHREE(int type1, int type2, int type3)
{
	Mtx44 rotate;

	GameObject::GAMEOBJECT_TYPE passedWall_1;
	GameObject::GAMEOBJECT_TYPE passedWall_2;
	GameObject::GAMEOBJECT_TYPE passedWall_3;

	//passedWall_1 is the higher rarity
	if (top_rarest == type1)
	{
		if (type1 == 1)
			passedWall_1 = GameObject::GO_RESOURCE1;
		if (type1 == 2)
			passedWall_1 = GameObject::GO_RESOURCE2;
		if (type1 == 3)
			passedWall_1 = GameObject::GO_RESOURCE3;
		if (type1 == 4)
			passedWall_1 = GameObject::GO_RESOURCE4;

		if (type2 == 1)
			passedWall_2 = GameObject::GO_RESOURCE1;
		if (type2 == 2)
			passedWall_2 = GameObject::GO_RESOURCE2;
		if (type2 == 3)
			passedWall_2 = GameObject::GO_RESOURCE3;
		if (type2 == 4)
			passedWall_2 = GameObject::GO_RESOURCE4;

		if (type3 == 1)
			passedWall_3 = GameObject::GO_RESOURCE1;
		if (type3 == 2)
			passedWall_3 = GameObject::GO_RESOURCE2;
		if (type3 == 3)
			passedWall_3 = GameObject::GO_RESOURCE3;
		if (type3 == 4)
			passedWall_3 = GameObject::GO_RESOURCE4;
	}
	else if(top_rarest == type2)
	{
		if (type2 == 1)
			passedWall_1 = GameObject::GO_RESOURCE1;
		if (type2 == 2)
			passedWall_1 = GameObject::GO_RESOURCE2;
		if (type2 == 3)
			passedWall_1 = GameObject::GO_RESOURCE3;
		if (type2 == 4)
			passedWall_1 = GameObject::GO_RESOURCE4;

		if (type1 == 1)
			passedWall_2 = GameObject::GO_RESOURCE1;
		if (type1 == 2)
			passedWall_2 = GameObject::GO_RESOURCE2;
		if (type1 == 3)
			passedWall_2 = GameObject::GO_RESOURCE3;
		if (type1 == 4)
			passedWall_2 = GameObject::GO_RESOURCE4;

		if (type3 == 1)
			passedWall_3 = GameObject::GO_RESOURCE1;
		if (type3 == 2)
			passedWall_3 = GameObject::GO_RESOURCE2;
		if (type3 == 3)
			passedWall_3 = GameObject::GO_RESOURCE3;
		if (type3 == 4)
			passedWall_3 = GameObject::GO_RESOURCE4;
	}
	else if (top_rarest == type3)
	{
		if (type3 == 1)
			passedWall_1 = GameObject::GO_RESOURCE1;
		if (type3 == 2)
			passedWall_1 = GameObject::GO_RESOURCE2;
		if (type3 == 3)
			passedWall_1 = GameObject::GO_RESOURCE3;
		if (type3 == 4)
			passedWall_1 = GameObject::GO_RESOURCE4;

		if (type1 == 1)
			passedWall_2 = GameObject::GO_RESOURCE1;
		if (type1 == 2)
			passedWall_2 = GameObject::GO_RESOURCE2;
		if (type1 == 3)
			passedWall_2 = GameObject::GO_RESOURCE3;
		if (type1 == 4)
			passedWall_2 = GameObject::GO_RESOURCE4;

		if (type2 == 1)
			passedWall_3 = GameObject::GO_RESOURCE1;
		if (type2 == 2)
			passedWall_3 = GameObject::GO_RESOURCE2;
		if (type2 == 3)
			passedWall_3 = GameObject::GO_RESOURCE3;
		if (type2 == 4)
			passedWall_3 = GameObject::GO_RESOURCE4;
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			GameObject* wall = FetchGO(passedWall_2);
			wall->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
			wall->scale.Set(30, 30, 15);
			wall->rotation.Set(0, 0, 55);
			rotate.SetToIdentity();
			rotate.SetToRotation(-55, 0, 0, 1);
			wall->normal = rotate* wall->normal;
			wall->b_lightEnabled = false;
			

			GameObject* wall2 = FetchGO(passedWall_2);
			wall2->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
			wall2->scale.Set(30, 30, 15);
			wall2->rotation.Set(0, 0, -55);
			rotate.SetToIdentity();
			rotate.SetToRotation(55, 0, 0, 1);
			wall2->normal = rotate* wall2->normal;
			wall2->b_lightEnabled = false;
		
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (i % 2 == 0)
			{
				GameObject* wall = FetchGO(passedWall_3);
				wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;
			

				GameObject* wall2 = FetchGO(passedWall_3);
				wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
				
			}
			else
			{
				GameObject* wall = FetchGO(passedWall_1);
				wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;
			

				GameObject* wall2 = FetchGO(passedWall_1);
				wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
			

			}

		}
	}
}

void CaptureGame::CreateTypeFOUR()
{
	Mtx44 rotate;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i % 2 == 0)
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE3);
				wall->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;
				wall->particleGenerator = &yellowbang;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE3);
				wall2->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
				wall2->particleGenerator = &yellowbang;
			}
			else
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE1);
				wall->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;
				wall->particleGenerator = &bluebang;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE1);
				wall2->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
				wall2->particleGenerator = &bluebang;
			}
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (i % 2 == 0)
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE2);
				wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;
				wall->particleGenerator = &redbang;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE2);
				wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
				wall2->particleGenerator = &redbang;
			}
			else
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE4);
				wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;
				wall->particleGenerator = &greenbang;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE4);
				wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
				wall2->particleGenerator = &greenbang;
			}
		}
	}

}

void CaptureGame::CreateScene()
{
	Mtx44 rotate;

	

	for (vector<ENEMY_TYPE>::iterator it = player.encounteredEnemies.begin(); it != player.encounteredEnemies.end(); ++it)
	{
		if (*it == ICE_MONSTER)
		{
			blueswitch = true;
		}
		if (*it == TANK)
		{
			redswitch = true;
		}
		if (*it == SPEED)
		{
			yellowswitch = true;
		}
		if (*it == MINION)
		{
			greenswitch = true;
		}
	
	
	}
	

	grass.meshID = GEO_DUNGEONWALL;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(2000, 2000, 2000);
	grass.rotation.Set(-90, 0, 0);
	grass.rotation.Set(0, 0, 0);
	grass.b_shadows = true;
	grass.b_lightEnabled = true;

	pot.meshID = GEO_POT;
	pot.pos.Set(0, 10, 0);					
	pot.scale.Set(500, 500, 500);
	pot.rotation.Set(0, 0, 0);
	pot.rotation.Set(0, 0, 0);
	pot.b_shadows = true;
	pot.b_lightEnabled = true;

	/*forValor.meshID = GEO_FOR_VALOR;
	forValor.pos.Set(0, 0.5, 0);
	forValor.scale.Set(2000, 2000, 2000);
	forValor.rotation.Set(-90, 0, 0);
	forValor.rotation.Set(0, 0, 0);
	forValor.b_shadows = false;*/
	
	// initialise values for particle generators
	bluebang.SetType(GEO_PARTICLE_BLUE);
	bluebang.SetFrequency(1);
	bluebang.SetCap(1000);
	bluebang.i_spawnAmount = 30;
	bluebang.f_lifeTime = 5.f;
	bluebang.minVel.Set(-900, -900, 0);
	bluebang.maxVel.Set(900, 900, 0);
	bluebang.scale.Set(10, 10, 10);
	bluebang.f_maxDist = 100.f;
	bluebang.isActive = false;

	redbang.SetType(GEO_PARTICLE_RED);
	redbang.SetFrequency(1);
	redbang.SetCap(1000);
	redbang.i_spawnAmount = 30;
	redbang.f_lifeTime = 5.f;
	redbang.minVel.Set(-900, -900, 0);
	redbang.maxVel.Set(900, 900, 0);
	redbang.scale.Set(10, 10, 10);
	redbang.f_maxDist = 100.f;
	redbang.isActive = false;

	yellowbang.SetType(GEO_PARTICLE_YELLOW);
	yellowbang.SetFrequency(1);
	yellowbang.SetCap(1000);
	yellowbang.i_spawnAmount = 30;
	yellowbang.f_lifeTime = 5.f;
	yellowbang.minVel.Set(-900, -900, 0);
	yellowbang.maxVel.Set(900, 900, 0);
	yellowbang.scale.Set(10, 10, 10);
	yellowbang.f_maxDist = 100.f;
	yellowbang.isActive = false;

	greenbang.SetType(GEO_PARTICLE_GREEN);
	greenbang.SetFrequency(1);
	greenbang.SetCap(1000);
	greenbang.i_spawnAmount = 30;
	greenbang.f_lifeTime = 5.f;
	greenbang.minVel.Set(-900, -900, 0);
	greenbang.maxVel.Set(900, 900, 0);
	greenbang.scale.Set(10, 10, 10);
	greenbang.f_maxDist = 100.f;
	greenbang.isActive = false;


	//CreateTypeFOUR();
	SwitchInitializer();

	//left right bounds
	GameObject* wall = FetchGO(GameObject::GO_WALL);
	wall->pos.Set(500, 0, 0);
	wall->scale.Set(50, 2000, 15);
	wall->normal.Set(1, 0, 0);
	wall->rotation.Set(0, 0, -180);

	GameObject* wall2 = FetchGO(GameObject::GO_WALL);
	wall2->pos.Set(-500, 0, 0);
	wall2->scale.Set(50, 2000, 15);
	wall2->normal.Set(1, 0, 0);
	wall2->rotation.Set(0, 0, -180);

	//bottom divider

	GameObject* pad = FetchGO(GameObject::GO_WALL);
	pad->pos.Set(0, -620, 0);
	pad->scale.Set(200, 50, 15);
	pad->rotation.Set(0, 0, 0);

	
}

void CaptureGame::ClearScene()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->b_isActive)
		{
			go->b_isActive = false;
		}
	}
}


void CaptureGame::Exit()
{
	for (vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		delete (*it);
	}
	m_goList.clear();

	
	whitebang.ClearParticles();
	bluebang.ClearParticles();
	redbang.ClearParticles();
	yellowbang.ClearParticles();
	greenbang.ClearParticles();

}
