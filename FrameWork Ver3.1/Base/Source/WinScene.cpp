
#include "WinScene.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"




WinScene::WinScene() :Scene()
{
}

WinScene::~WinScene()
{
}

void WinScene::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	CreateScene();
	camera.Init(Vector3(0, 0, 1500), Vector3(0, 0, 0), Vector3(0, 1, 0),1);
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	std::cout << "entered";

}

void WinScene::Update(double dt)
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

	whiteUp.Update(dt);
	whiteUp2.Update(dt);

	whiteUp.SetType(GEO_PARTICLE_WHITE);
	//whiteUp.pos.Set(0, -300, 0);
	whiteUp.SetFrequency(1);
	whiteUp.SetCap(1000);
	whiteUp.i_spawnAmount = 5;
	whiteUp.f_lifeTime = 15.f;
	whiteUp.minVel.Set(-50, 5, 0);
	whiteUp.maxVel.Set(50, 500, 0);
	whiteUp.scale.Set(20, 20, 1);
	whiteUp.f_maxDist = 1000.f;
	whiteUp.isActive = false;
	whiteUp.SpawnParticle(Vector3(500, -500, 1));

	whiteUp2.SetType(GEO_PARTICLE_WHITE);
	//whiteUp.pos.Set(0, -300, 0);
	whiteUp2.SetFrequency(1);
	whiteUp2.SetCap(1000);
	whiteUp2.i_spawnAmount = 5;
	whiteUp2.f_lifeTime = 15.f;
	whiteUp2.minVel.Set(-50, 5, 0);
	whiteUp2.maxVel.Set(50, 500, 0);
	whiteUp2.scale.Set(20, 20, 1);
	whiteUp2.f_maxDist = 1000.f;
	whiteUp2.isActive = false;
	whiteUp2.SpawnParticle(Vector3(-600, -500, 1));


	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);

	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(8, false);
	}
	
}

void WinScene::Render()
{
	int spacing = 13;

	RenderManager::GetInstance()->RenderTextOnScreen("YOU ", Color(1, 1, 0), 15, 25, 30);
	RenderManager::GetInstance()->RenderTextOnScreen("WIN ", Color(1, 1, 0), 15, 27, 20);
	//RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(balls), Color(1, 1, 1), 2, spacing, 40);
}

void WinScene::CreateScene()
{
	Mtx44 rotate;

	grass.meshID = GEO_DUNGEONWALL;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(2000, 2000, 2000);
	grass.rotation.Set(-90, 0, 0);
	grass.rotation.Set(0, 0, 0);
	grass.b_shadows = false;
	grass.b_lightEnabled = false;


}




void WinScene::Exit()
{

	whiteUp.ClearParticles();
	whiteUp2.ClearParticles();
}
