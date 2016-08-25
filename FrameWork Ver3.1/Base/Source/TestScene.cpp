#include "TestScene.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"

TestScene::TestScene( ) :Scene()
{
}

TestScene::~TestScene()
{
}

void TestScene::Init()
{
	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(1000, 1000, 1000);
	grass.rotation.Set(-90, 0, 0);

	camera.Init(Vector3(120, 100, 200), Vector3(0, 50, 0), Vector3(0, 1, 0));
	RenderManager::GetInstance()->SetCamera(&camera);
	RenderManager::GetInstance()->SetLight(Vector3(-0.5, -0.5, 1));
}

void TestScene::Update(double dt)
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

	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(1, false);
	}



	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);


}

void TestScene::Render()
{
	
}

void TestScene::Exit()
{
	//clean Up scene Variables

}
