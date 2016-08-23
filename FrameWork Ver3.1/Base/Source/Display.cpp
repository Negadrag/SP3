
#include "Display.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"




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
	
	camera.Init(Vector3(0, 700, 700), Vector3(0, 10, 0), Vector3(0, 1, 0), 2);
	camera.b_ortho = false;
	camera.farPlane = 100000.f;
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	std::cout << "entered";
	CreateScene();
	
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


	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(1, false);
	}

}

void Display::Render()
{
	int spacing = 13;

	//RenderManager::GetInstance()->RenderTextOnScreen("YOU ", Color(1, 0, 0), 15, 25, 30);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testx), Color(1, 0, 0), 3, 25, 30);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testy), Color(1, 0, 0), 3, 25, 25);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testz), Color(1, 0, 0), 3, 25, 20);

}

void Display::CreateScene()
{
	Mtx44 rotate;

	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(2000, 2000, 2000);
	grass.rotation.Set(-90, 0, 0);
	//grass.rotation.Set(0, 0, 0);
	grass.b_shadows = false;
	grass.b_lightEnabled = false;

	demoObject.meshID = GEO_ICETOWER;
	demoObject.pos.Set(0, 0, 0);
	demoObject.rotation.Set(-90, 0, 0);
	demoObject.scale.Set(200, 200, 200);
	demoObject.b_shadows = false;
	demoObject.b_lightEnabled = false;

	CreateSkybox();


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
}
