
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
#include "Boss.h"

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

	testx = testy = testz = 1;
	banner_forward = -45;
	banner_backward = 80;

	descriptscaleX = 1;
	descriptscaleY = 1;

	camera.Init(Vector3(0, 7, 7), Vector3(0, 2, 0), Vector3(0, 1, 0), 2);
	camera.b_ortho = false;
	camera.farPlane = 100000.f;

	RenderManager::GetInstance()->SetCamera(&camera);

	b_opendescript = false;
	b_showdescript = false;
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
		testx += dt * 50;
	if (Application::IsKeyPressed('J'))
		testx -= dt * 50;
	if (Application::IsKeyPressed('I'))
		testy += dt * 50;
	if (Application::IsKeyPressed('K'))
		testy -= dt * 50;
	if (Application::IsKeyPressed('P'))
		testz += dt * 50;
	if (Application::IsKeyPressed('O'))
		testz -= dt * 50;

	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);

	if (camera.showcase_intro == true)
	{
//		descriptBG->b_isActive = true;
	//	descriptBG->b_textActive = true;

		b_opendescript = true;
		//descriptBG->scale.x = testy;	//20
		//descriptBG->scale.y = testx;	//30
	}
	if (b_opendescript == true)
	{
		if (descriptscaleY < 20)
		{
			descriptscaleY += 50 * dt;
		}
		else if (descriptscaleX < 30)
		{
			descriptscaleX += 40 * dt;
		}
		else
			b_showdescript = true;

	}

	float skyboxsize = 10000.f;
	if (f_timer >= 3.f)
	{
		b_skipDebounce = true;
	}



	if (b_skipDebounce == true && (Application::IsKeyPressed('C') || f_timer >= 30.f))
	{
		SceneManager::GetInstance()->ReinstanceScene(7);
	}
	f_timer += dt;
}

void Display::Render()
{
	int spacing = 13;


	BannerManager(!player.b_showcaseEnemy, true);
	RenderManager::GetInstance()->RenderMeshOnScreen(GEO_ADJUSTBAR, false, Vector3(65, 45, -5), Vector3(descriptscaleX, descriptscaleY, 1), Vector3(0, 0, -90));

	if (b_showdescript)
	{
		switch (i_diplayedObject)
		{
		case 1:
			RenderManager::GetInstance()->RenderTextOnScreen("Minion", Color(0.3, 1, 0.3), 3, 60, 40);
			RenderManager::GetInstance()->RenderTextOnScreen("Speed:   Med", Color(0.3, 1, 0.3), 2, 55, 33);
			RenderManager::GetInstance()->RenderTextOnScreen("Base HP:   50", Color(0.3, 1, 0.3), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("Def:   10", Color(0.3, 1, 0.3), 2, 55, 27);
			RenderManager::GetInstance()->RenderTextOnScreen("Notes: ", Color(0.3, 1, 0.3), 2, 55, 24);
			RenderManager::GetInstance()->RenderTextOnScreen("  Immune to poison!", Color(0.3, 1, 0.3), 2, 55, 21);
			break;
		case 2:
			RenderManager::GetInstance()->RenderTextOnScreen("IceCrystal", Color(0.6, 0.6, 1), 3, 57, 40);
			RenderManager::GetInstance()->RenderTextOnScreen("Speed:   Med", Color(0.6, 0.6, 1), 2, 55, 33);
			RenderManager::GetInstance()->RenderTextOnScreen("Base HP:   50", Color(0.6, 0.6, 1), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("Def:   15", Color(0.6, 0.6, 1), 2, 55, 27);
			RenderManager::GetInstance()->RenderTextOnScreen("Notes: ", Color(0.6, 0.6, 1), 2, 55, 24);
			RenderManager::GetInstance()->RenderTextOnScreen("  Immune to slow!", Color(0.6, 0.6, 1), 2, 55, 21);
			break;
		case 3:
			RenderManager::GetInstance()->RenderTextOnScreen("Tanker", Color(1, 0.4, 0.4), 3, 60, 40);
			RenderManager::GetInstance()->RenderTextOnScreen("Speed:   Slow", Color(1, 0.4, 0.4), 2, 55, 33);
			RenderManager::GetInstance()->RenderTextOnScreen("Base HP:   100", Color(1, 0.4, 0.4), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("Def:   30", Color(1, 0.4, 0.4), 2, 55, 27);
			RenderManager::GetInstance()->RenderTextOnScreen("Notes: ", Color(1, 0.4, 0.4), 2, 55, 24);
			RenderManager::GetInstance()->RenderTextOnScreen(" Its slow but thick.", Color(1, 0.4, 0.4), 2, 55, 21);
			break;
		case 4:
			RenderManager::GetInstance()->RenderTextOnScreen("Speeder", Color(0.6, 0.6, 0.6), 3, 60, 40);
			RenderManager::GetInstance()->RenderTextOnScreen("Speed:   Fast", Color(0.6, 0.6, 0.6), 2, 55, 33);
			RenderManager::GetInstance()->RenderTextOnScreen("Base HP:   30", Color(0.6, 0.6, 0.6), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("Def:   0", Color(0.6, 0.6, 0.6), 2, 55, 27);
			RenderManager::GetInstance()->RenderTextOnScreen("Notes: ", Color(0.6, 0.6, 0.6), 2, 55, 24);
			RenderManager::GetInstance()->RenderTextOnScreen("  Its fast but weak.", Color(0.6, 0.6, 0.6), 2, 55, 21);
			break;
		case 5:
			RenderManager::GetInstance()->RenderTextOnScreen("BOSS", Color(0.6, 0.6, 0.6), 3, 60, 40);
			RenderManager::GetInstance()->RenderTextOnScreen("Speed:   Slow", Color(0.6, 0.6, 0.6), 2, 55, 33);
			RenderManager::GetInstance()->RenderTextOnScreen("Base HP: Too high!", Color(0.6, 0.6, 0.6), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("Def: Also very high!", Color(0.6, 0.6, 0.6), 2, 55, 27);
			RenderManager::GetInstance()->RenderTextOnScreen("Notes: ", Color(0.6, 0.6, 0.6), 2, 55, 24);
			RenderManager::GetInstance()->RenderTextOnScreen("  The jumping thing.", Color(0.6, 0.6, 0.6), 2, 55, 21);
			break;
		case 11:
			RenderManager::GetInstance()->RenderTextOnScreen("PoisonTower", Color(0.3, 1, 0.3), 3, 57, 41);
			RenderManager::GetInstance()->RenderTextOnScreen("Description: ", Color(0.3, 1, 0.3), 2, 55, 35);
			RenderManager::GetInstance()->RenderTextOnScreen("Does damage while ", Color(0.3, 1, 0.3), 2, 55, 32);
			RenderManager::GetInstance()->RenderTextOnScreen("poisoning enemies", Color(0.3, 1, 0.3), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("which slows and ", Color(0.3, 1, 0.3), 2, 55, 28);
			RenderManager::GetInstance()->RenderTextOnScreen("reduces Hp over time ", Color(0.3, 1, 0.3), 2, 55, 26);
			RenderManager::GetInstance()->RenderTextOnScreen("(Poison ignores defence!) ", Color(0.3, 1, 0.3), 1.5f, 55, 19);
			break;
		case 12:
			RenderManager::GetInstance()->RenderTextOnScreen("IceTower", Color(0.6, 0.6, 1), 3, 59, 41);
			RenderManager::GetInstance()->RenderTextOnScreen("Description: ", Color(0.6, 0.6, 1), 2, 55, 35);
			RenderManager::GetInstance()->RenderTextOnScreen("Very Low Damage ", Color(0.6, 0.6, 1), 2, 55, 32);
			RenderManager::GetInstance()->RenderTextOnScreen("but slows enemies", Color(0.6, 0.6, 1), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("more than the ", Color(0.6, 0.6, 1), 2, 55, 28);
			RenderManager::GetInstance()->RenderTextOnScreen("Poison towers ", Color(0.6, 0.6, 1), 2, 55, 26);
			RenderManager::GetInstance()->RenderTextOnScreen(" ", Color(0.6, 0.6, 1), 1.5f, 55, 19);
			break;
		case 13:
			RenderManager::GetInstance()->RenderTextOnScreen("MortarTower", Color(0.1, 0.7, 0.7), 3, 57, 41);
			RenderManager::GetInstance()->RenderTextOnScreen("Description: ", Color(0.1, 0.7, 0.7), 2, 55, 35);
			RenderManager::GetInstance()->RenderTextOnScreen("Does super heavy ", Color(0.1, 0.7, 0.7), 2, 55, 32);
			RenderManager::GetInstance()->RenderTextOnScreen("damage. But slow", Color(0.1, 0.7, 0.7), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("to recharge  ", Color(0.1, 0.7, 0.7), 2, 55, 28);
			RenderManager::GetInstance()->RenderTextOnScreen(" ", Color(0.1, 0.7, 0.7), 2, 55, 26);
			RenderManager::GetInstance()->RenderTextOnScreen(" ", Color(0.1, 0.7, 0.7), 1.5f, 55, 19);
			break;
		case 14:
			RenderManager::GetInstance()->RenderTextOnScreen("SpeedTower", Color(0.7, 0.7, 0.7), 3, 57, 41);
			RenderManager::GetInstance()->RenderTextOnScreen("Description: ", Color(0.7, 0.7, 0.7), 2, 55, 35);
			RenderManager::GetInstance()->RenderTextOnScreen("This tower has ", Color(0.7, 0.7, 0.7), 2, 55, 32);
			RenderManager::GetInstance()->RenderTextOnScreen("very fast shooting", Color(0.7, 0.7, 0.7), 2, 55, 30);
			RenderManager::GetInstance()->RenderTextOnScreen("speed but low ", Color(0.7, 0.7, 0.7), 2, 55, 28);
			RenderManager::GetInstance()->RenderTextOnScreen("damage ", Color(0.7, 0.7, 0.7), 2, 55, 26);
			RenderManager::GetInstance()->RenderTextOnScreen(" ", Color(0.7, 0.7, 0.7), 1.5f, 55, 19);
			break;
		}

	}

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
	grass.scale.Set(30, 30, 30);
	grass.rotation.Set(-90, 0, 0);
	//grass.rotation.Set(0, 0, 0);
	grass.b_shadows = true;
	grass.b_lightEnabled = false;





	if (player.i_showcaseIndex >= player.enemyToShowcase.size())
	{
		SceneManager::GetInstance()->ChangeScene(player.m_sceneID, false);
		player.i_showcaseIndex = 0;
		if (player.b_showcaseEnemy == false)
		{
			player.enemyToShowcase.clear();
		}

		
	}
	if (player.b_showcaseEnemy == true)
	{
		if (player.enemyToShowcase.empty() == false)
		{
			
			ENEMY_TYPE showcase = player.enemyToShowcase[player.i_showcaseIndex];
			Vector3 pos(0, 1, 0);
			if (showcase == MINION)
			{
				demoObject = new Minion(pos, nullptr);
				i_diplayedObject = 1;
			}
			else if (showcase == ICE_MONSTER)
			{
				demoObject = new IceMonster(pos, nullptr);
				i_diplayedObject = 2;
			}
			else if (showcase == TANK)
			{
				demoObject = new TankMonster(pos, nullptr);
				i_diplayedObject = 3;
			}
			else if (showcase == SPEED)
			{
				demoObject = new SpeedMonster(pos, nullptr);
				i_diplayedObject = 4;
			}
			if (showcase == BOSS)
			{
				demoObject = new Boss(pos, nullptr, nullptr);
				i_diplayedObject = 5;
			}
			demoObject->rotation.x = -90.f;
			demoObject->scale.Set(2, 2, 2);
		}

	}
	else
	{
		
		
		if (player.enemyToShowcase.empty() == false)
		{
			ENEMY_TYPE showcase = player.enemyToShowcase[player.i_showcaseIndex];
			Vector3 pos(0, 1, 0);
			if (showcase == MINION)
			{
				demoObject = new PoisonTower();
				i_diplayedObject = 11;
			}
			else if (showcase == ICE_MONSTER)
			{
				demoObject = new IceTower();
				i_diplayedObject = 12;
			}
			else if (showcase == TANK)
			{
				demoObject = new MortarTower();
				i_diplayedObject = 13;
			}
			else if (showcase == SPEED)
			{
				demoObject = new SpeedTower();
				i_diplayedObject = 14;
			}
			else if (showcase == BOSS)
			{
				SceneManager::GetInstance()->ReinstanceScene(7);
				player.i_showcaseIndex++;
			}
			if (demoObject != nullptr)
			{
				demoObject->rotation.x = -90.f;
			}
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
