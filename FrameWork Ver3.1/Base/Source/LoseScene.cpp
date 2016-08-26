
#include "LoseScene.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"




LoseScene::LoseScene() :Scene()
{
}

LoseScene::~LoseScene()
{
}

void LoseScene::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	CreateScene();
	camera.Init(Vector3(0, 0, 1500), Vector3(0, 0, 0), Vector3(0, 1, 0),1);
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	std::cout << "entered";

}

void LoseScene::Update(double dt)
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

	redfall.Update(dt);
	

	redfall.SetType(GEO_PARTICLE_RED);
	redfall.SetFrequency(1);
	redfall.SetCap(1000);
	redfall.i_spawnAmount = 1;
	redfall.f_lifeTime = 15.f;
	redfall.minVel.Set(-800, -100, 0);
	redfall.maxVel.Set(800, -100, 0);
	redfall.scale.Set(20, 20, 20);
	redfall.f_maxDist = 1500.f;
	redfall.isActive = false;
	redfall.SpawnParticle(Vector3(0, 1000, 1));




	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);

	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(8, false);
	}
	/*if (Application::IsKeyPressed(VK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene(7, false);
	}*/

}

void LoseScene::Render()
{
	int spacing = 13;

	RenderManager::GetInstance()->RenderTextOnScreen("YOU ", Color(1, 0, 0), 15, 25, 30);
	RenderManager::GetInstance()->RenderTextOnScreen("LOSE ", Color(1, 0, 0), 15, 23, 20);
	//RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(balls), Color(1, 1, 1), 2, spacing, 40);


}

void LoseScene::CreateScene()
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




void LoseScene::Exit()
{

	redfall.ClearParticles();
}
