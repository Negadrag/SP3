
#include "Display.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"

#include "Minion.h"
#include "IceMonster.h"
#include "TankMonster.h"
#include "SpeedMonster.h"

#include "IceTower.h"
#include "PoisonTower.h"
#include "MortarTower.h"
#include "SpeedTower.h"

Display::Display() :Scene()
{
}

Display::~Display()
{
}

void Display::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	testx = testy = testz = 0;
	banner_forward = -45;
	banner_backward = 80;

	camera.Init(Vector3(0, 7, 7), Vector3(0, 2, 0), Vector3(0, 1, 0), 2);
	camera.b_ortho = false;
	camera.farPlane = 100000.f;

	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	//std::cout << "entered";
	CreateScene();
	RenderManager::GetInstance()->SetLight(Vector3(0, 2, 0));
}

void Display::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	banner_forward += dt * 30;
	if (banner_forward >= 80)
		banner_forward = -45;
	banner_backward -= dt * 30;
	if (banner_backward <= -45)
		banner_backward = 80;


	fps = (float)(1.f / dt);

	/*redfall.Update(dt);


	redfall.SetType(GEO_PARTICLE_RED);
	redfall.SetFrequency(1);
	redfall.SetCap(1000);
	redfall.i_spawnAmount = 1;
	redfall.f_lifeTime = 15.f;
	redfall.minVel.Set(-800, -100, 0);
	redfall.maxVel.Set(800, -100, 0);
	redfall.scale.Set(10, 10, 10);
	redfall.f_maxDist = 1500.f;
	redfall.isActive = false;
	redfall.SpawnParticle(Vector3(0, 1000, 1));*/


	if (Application::IsKeyPressed('L'))
		testx += dt * 100;
	if (Application::IsKeyPressed('J'))
		testx -= dt * 100;
	if (Application::IsKeyPressed('I'))
		testy += dt * 100;
	if (Application::IsKeyPressed('K'))
		testy -= dt * 100;
	if (Application::IsKeyPressed('P'))
		testz += dt * 100;
	if (Application::IsKeyPressed('O'))
		testz -= dt * 100;

	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);

	float skyboxsize = 10000.f;
	if (f_timer >= 3.f)
	{
		b_skipDebounce = true;
	}

	//if (Application::IsKeyPressed('M'))
	//{
	//	SceneManager::GetInstance()->ChangeScene(1, false);
	//}

	if (b_skipDebounce == true && (Application::IsKeyPressed('C') || f_timer >= 30.f))
	{
		SceneManager::GetInstance()->ReinstanceScene(7);
	}
	f_timer += dt;
}

void Display::Render()
{
	int spacing = 13;

	//-45 , 80
	//RenderManager::GetInstance()->RenderTextOnScreen("YOU ", Color(1, 0, 0), 15, 25, 30);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.x), Color(1, 0, 0), 2, 15, 20);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.y), Color(1, 0, 0), 2, 15, 15);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.z), Color(1, 0, 0), 2, 15, 10);

	BannerManager(!player.b_showcaseEnemy, true);

	if (camera.showcase_intro == true)
		RenderManager::GetInstance()->RenderTextOnScreen("Press C to continue.", Color(1, 1, 0), 3, 20, 0);
}

void Display::BannerManager(bool tower, bool enemy)
{
	if (tower)
	{
		RenderManager::GetInstance()->RenderTextOnScreen("NEW TOWER ", Color(1, 1, 0), 10, banner_forward, 50);
		RenderManager::GetInstance()->RenderTextOnScreen("NEW TOWER ", Color(1, 1, 0), 10, banner_backward, 3);
	}

	else if (enemy)
	{
		RenderManager::GetInstance()->RenderTextOnScreen("WARNING ", Color(1, 0.2, 0.2), 10, banner_forward, 50);
		RenderManager::GetInstance()->RenderTextOnScreen("NEW ENEMY ", Color(1, 0.2, 0.2), 10, banner_backward, 3);
	}

	else
	{
		RenderManager::GetInstance()->RenderTextOnScreen("? ? ? ", Color(1, 1, 1), 10, banner_forward, 50);
		RenderManager::GetInstance()->RenderTextOnScreen("? ? ? ", Color(1, 1, 1), 10, banner_backward, 3);
	}
}

void Display::CreateScene()
{


	f_timer = 0.f;
	b_skipDebounce = false;

	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(20, 20, 20);
	grass.rotation.Set(-90, 0, 0);
	//grass.rotation.Set(0, 0, 0);
	grass.b_shadows = true;
	grass.b_lightEnabled = false;

	if (player.b_showcaseEnemy == true)
	{
		if (player.i_showcaseIndex >= player.enemyToShowcase.size())
		{
			player.i_showcaseIndex = 0;
			SceneManager::GetInstance()->ChangeScene(player.m_sceneID, false);
		}
		ENEMY_TYPE showcase = player.enemyToShowcase[player.i_showcaseIndex];
		Vector3 pos(0, 1, 0);
		if (showcase == MINION)
		{
			demoObject = new Minion(pos, nullptr);
		}
		else if (showcase == ICE_MONSTER)
		{
			demoObject = new IceMonster(pos, nullptr);
		}
		else if (showcase == TANK)
		{
			demoObject = new TankMonster(pos, nullptr);
		}
		else if (showcase == SPEED)
		{
			demoObject = new SpeedMonster(pos, nullptr);
		}
		demoObject->rotation.x = -90.f;
		demoObject->scale.Set(2, 2, 2);
	}
	else
	{
		if (player.i_showcaseIndex >= player.enemyToShowcase.size())
		{
			SceneManager::GetInstance()->ChangeScene(player.m_sceneID, false);
			player.i_showcaseIndex = 0;
			player.enemyToShowcase.clear();
		}
		if (player.enemyToShowcase.empty() == false)
		{
			ENEMY_TYPE showcase = player.enemyToShowcase[player.i_showcaseIndex];
			Vector3 pos(0, 1, 0);
			if (showcase == MINION)
			{
				demoObject = new PoisonTower();
			}
			else if (showcase == ICE_MONSTER)
			{
				demoObject = new IceTower();
			}
			else if (showcase == TANK)
			{
				demoObject = new MortarTower();
			}
			else if (showcase == SPEED)
			{
				demoObject = new SpeedTower();
			}
			demoObject->rotation.x = -90.f;
		}
		
	}

	
	CreateSkybox();
	player.i_showcaseIndex++;

}

void Display::CreateSkybox()
{
	float skyboxsize = 10000.f;

	skytop.meshID = GEO_SKYTOP;
	skytop.pos.Set(0, 5000, 0);
	skytop.rotation.Set(90, 0, 0);
	skytop.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skytop.b_shadows = false;
	skytop.b_lightEnabled = false;

	skyfront.meshID = GEO_SKYFRONT;
	skyfront.rotation.Set(0, 90, 0);
	skyfront.pos.Set(-5000, 0, 0);
	skyfront.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyfront.b_shadows = false;
	skyfront.b_lightEnabled = false;

	
	skyright.meshID = GEO_SKYRIGHT;
	skyright.pos.Set(0, 0, -skyboxsize / 2 + 2.f);
	skyright.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyright.b_shadows = false;
	skyright.b_lightEnabled = false;

	skyleft.meshID = GEO_SKYLEFT;
	skyleft.rotation.Set(0, 180, 0);
	skyleft.pos.Set(0, 0, 5000);
	skyleft.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyleft.b_shadows = false;
	skyleft.b_lightEnabled = false;

	skyback.meshID = GEO_SKYBACK;
	skyback.rotation.Set(0, -90, 0);
	skyback.pos.Set(5000, 0, 0);
	skyback.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyback.b_shadows = false;
	skyback.b_lightEnabled = false;

	skybottom.meshID = GEO_SKYBOTTOM;
	skybottom.rotation.Set(-90, 0, 0);
	skybottom.pos.Set(0, -5000, 0);
	skybottom.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skybottom.b_shadows = false;
	skybottom.b_lightEnabled = false;



}


void Display::Exit()
{

	redfall.ClearParticles();
	if (demoObject != nullptr)
	{
		delete demoObject;
	}
	demoObject = nullptr;
}
