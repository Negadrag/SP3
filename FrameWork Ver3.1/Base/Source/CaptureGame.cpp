#include "CaptureGame.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"

CaptureGame::CaptureGame(int sceneId) :Scene(sceneId)
{
}

CaptureGame::~CaptureGame()
{
}

void CaptureGame::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(1000, 1000, 1000);
	grass.rotation.Set(-90, 0, 0);


	camera.Init(Vector3(120, 100, 200), Vector3(0, 50, 0), Vector3(0, 1, 0));
	RenderManager::GetInstance()->SetCamera(&camera);

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



	fps = (float)(1.f / dt);


	//if (Application::IsKeyPressed(VK_SPACE))
	//{
	//	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//	m_sceneID = 1;
	//}


	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);


}

void CaptureGame::Render()
{
}

void CaptureGame::Exit()
{
	//clean Up scene Variables

}
