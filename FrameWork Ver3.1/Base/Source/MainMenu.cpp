
#include "MainMenu.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"
#include "Music.h"


MainMenu::MainMenu() :Scene()
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	this->Init2();

	testx = testy = testz = 0;
	banner_forward = -45;
	banner_backward = 80;

	camera.Init(Vector3(0, 4, 11), Vector3(0, 4, 10), Vector3(0, 1, 0), 1);
	camera.b_ortho = false;
	camera.farPlane = 100000.f;
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	std::cout << "entered";
	
	CreateScene();
	

	cursor.Init(&camera);

}

void MainMenu::Init2()
{
	Music::GetInstance()->PlayMusic(6, true, 0.1f);
}

void MainMenu::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




	cursor.Update(dt);


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

	waterfountain.Update(dt);
	

	waterfountain.SetType(GEO_PARTICLE_BLUE);
	waterfountain.SetFrequency(1);
	waterfountain.SetCap(1000);
	waterfountain.i_spawnAmount = 1;
	waterfountain.f_lifeTime = 1.f;
	waterfountain.minVel.Set(-4, 10, -4);
	waterfountain.maxVel.Set(4, 30, 4);
	waterfountain.scale.Set(2, 2, 2);
	waterfountain.f_maxDist = 1500.f;
	waterfountain.b_gravity = true;
	waterfountain.isActive = false;
	waterfountain.SpawnParticle(Vector3(-0.4f, 2.f, -119.5f));

	

	if (Application::IsKeyPressed('L'))
		testx += dt * 10.f;
	if (Application::IsKeyPressed('J'))
		testx -= dt * 10.f;
	if (Application::IsKeyPressed('I'))
		testy += dt * 10.f;
	if (Application::IsKeyPressed('K'))
		testy -= dt * 10.f;
	if (Application::IsKeyPressed('P'))
		testz += dt * 10.f;
	if (Application::IsKeyPressed('O'))
		testz -= dt * 10.f;

	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);

	float skyboxsize = 10000.f;


	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(1, false);
	}

}

void MainMenu::Render()
{
	int spacing = 13;

	//-45 , 80
	RenderManager::GetInstance()->RenderTextOnScreen("CAPTURE DEFENCE ", Color(0.7f, 0.7f, 1.f), 5, 18, 50);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(fps), Color(1, 1, 1), 2, 45, 55);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testx), Color(1, 1, 1), 2, 45, 15);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testy), Color(1, 1, 1), 2, 45, 10);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testz), Color(1, 1, 1), 2, 45, 5);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.x), Color(1, 0, 0), 2, 65, 35);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.y), Color(1, 0, 0), 2, 65, 30);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.z), Color(1, 0, 0), 2, 65, 25);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.target.x), Color(1, 0, 0), 2, 65, 15);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.target.y), Color(1, 0, 0), 2, 65, 10);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.target.z), Color(1, 0, 0), 2, 65, 5);


	/*cam pos
	main = 0 , 4 , 10
	black = -27 , 4 , 10
	alley = 58, 4 , 10
	square = 0, 4 , -90*/
	
	
	
	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(10, 0, -80);
	grass.scale.Set(140, 200, 140);
	grass.rotation.Set(-90, 0, 0);
	//grass.rotation.Set(0, 0, 0);
	grass.b_shadows = true;
	grass.b_lightEnabled = false;
}


void MainMenu::CreateScene()
{
	
	Mtx44 rotate;


	ren_menutown.meshID = GEO_MENUTOWN;
	ren_menutown.pos.Set(3, 0, 0);
	//demo_main.rotation.Set(-90, 0, 0);
	ren_menutown.scale.Set(1, 1, 1);
	ren_menutown.b_shadows = true;
	ren_menutown.b_lightEnabled = true;

	CreateSkybox();
	//SceneDeco();
	


}

void MainMenu::CreateSkybox()
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

void MainMenu::SceneDeco()
{
	




}




void MainMenu::Exit()
{

	redfall.ClearParticles();
}
