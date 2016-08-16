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

Assignment::Assignment(int sceneID) :Scene(sceneID)
{
}

Assignment::~Assignment()
{
}

void Assignment::Init()
{
	//this->m_sceneID = 1;
	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(1000, 1000, 1000);
	grass.rotation.Set(-90, 0, 0);

	testball.meshID = GEO_SPHERE;
	testball.pos.Set(0, 10, 0);
	testball.scale.Set(10, 10, 10);

	turntab.Init(Vector3(200,300,400), Vector3(0,-20,0), Vector3(0,1,0));
	//camera.Init(Vector3(120, 100, 200), Vector3(0, 50, 0), Vector3(0, 1, 0));
	RenderManager::GetInstance()->SetCamera(&turntab);

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
	

	if (Application::IsKeyPressed('N'))
	{
		//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		SceneManager::GetInstance()->ChangeScene(2, true);
	}

	
	fps = (float)(1.f / dt);


	turntab.Update(dt);
	//camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&turntab);


}

void Assignment::Render()
{
}

void Assignment::Exit()
{
	//clean Up scene Variables
	
}
