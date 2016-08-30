#include "PauseCursor.h"
#include "Application.h"
#include "MathUtility.h"
#include "SceneManager.h"


PauseCursor::PauseCursor()
{
	bLButtonState = false;
}

PauseCursor::~PauseCursor()
{
	Clear();
}

void PauseCursor::Init()
{
	temp = new GUI("Paused (ESC to resume,M to Main Menu)");
	temp->position.Set(2, 28);
	temp->SetTextSize(4);
}

void PauseCursor::Update(OrthoCamera &camera, const double &dt)
{
	Cursor::Update(camera, dt);

	Clicking();
	HotKeys();
}

void PauseCursor::HotKeys()
{
	if (Application::IsKeyPressed(VK_ESCAPE))
	{
		SceneManager::GetInstance()->ChangeScene(Scene::player.m_sceneID, false);
	}
	else if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ReinstanceScene(Scene::player.m_sceneID);
		SceneManager::GetInstance()->ChangeScene(8, false);
	}
}

void PauseCursor::Clicking()
{
}

void PauseCursor::Clear()
{
	if (temp != nullptr)
	{
		delete temp;
	}
}