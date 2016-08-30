#include "Pause.h"
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


Pause::Pause() :Scene()
{
}

Pause::~Pause()
{
}

void Pause::Init()
{
	this->Init2();
	//this->m_sceneID = 1;


	camera = *player.tempCamera;
	cursor.Init();
	//camera.Init(Vector3(0,-5,10), Vector3(0,0,0), Vector3(0, 1, 0));

	RenderManager::GetInstance()->SetCamera(&camera);

}

void Pause::Init2()
{
	//Music::GetInstance()->PlayMusic(6, true, 0.2f);
}

void Pause::Update(double dt)
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
	camera.Update(dt);
	cursor.Update(camera, dt);

	RenderManager::GetInstance()->SetLight(Vector3(-0.5, -0.5, 1));

	RenderManager::GetInstance()->SetCamera(&camera);
}

void Pause::Render()
{
	int previousSceneID = player.m_sceneID;

	RenderManager::GetInstance()->RenderGPass(previousSceneID);

	RenderManager::GetInstance()->RenderMain(previousSceneID);
	for (list<Scene*>::iterator it = SceneManager::GetInstance()->sceneList.begin(); it != SceneManager::GetInstance()->sceneList.end(); ++it)
	{
		if ((*it)->m_sceneID == previousSceneID)
		{
			(*it)->Render();
		}
	}
}

void Pause::Exit()
{
	//clean Up scene Variables
}