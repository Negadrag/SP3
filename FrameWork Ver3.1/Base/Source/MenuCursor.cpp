#include "MenuCursor.h"
#include "Application.h"
#include "MathUtility.h"
#include "GUI.h"

MenuCursor::MenuCursor()
{
	bLButtonState = false;
}

MenuCursor::~MenuCursor()
{
}

void MenuCursor::Init(GameplayCam *camera)
{
	this->camera = camera;
	istransition = false;
	menu_state = 0;
}

void MenuCursor::Update(const double &dt)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	worldX = x / w - 0.5f; // -0.5 - 0.5
	worldY = 1.f - (y / h) - 0.5f; // -0.5 - 0.5

	static const float CAMERA_SPEED = 10.f;

	SceneMoveNorth(CAMERA_SPEED, dt);

	Clicking(dt);
	HotKeys();
}

void MenuCursor::HotKeys()
{

}

void MenuCursor::Clicking(double dt)
{
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;

		GUI* temp = GUIManager::GetInstance()->FindGUI(worldX, worldY);
		if (temp != nullptr)
		{
			static const float CAMERA_SPEED = 10.f;

			if (temp->functionID == 0)
			{
				std::cout << "BUTTON PLAY" << std::endl;
				
			}
			if (temp->functionID == 1)
			{
				std::cout << "BUTTON EDITING" << std::endl;

			}
			if (temp->functionID == 2)
			{
				std::cout << "BUTTON TUTORIAL" << std::endl;
				istransition = true;
				menu_state = 2;
			}
			if (temp->functionID == 3)
			{
				std::cout << "BUTTON OPTIONS" << std::endl;

			}
			if (temp->functionID == 4)
			{
				std::cout << "BUTTON EXIT" << std::endl;

			}


		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		istransition = true;
	}
}

void MenuCursor::SceneMoveNorth(float cam_spd, double dt)
{
	float turnCam_spd = cam_spd + 30;

	if (camera->position.z >= -20 && istransition)
	{
		Vector3 view = (camera->target - camera->position).Normalized();
		camera->position += view * turnCam_spd * (float)dt;
		camera->target += view * turnCam_spd * (float)dt;
	}
	else
		istransition = false;
}

void MenuCursor::SceneMoveRight(float cam_spd, double dt)
{

}

void MenuCursor::SceneMoveLeft(float cam_spd, double dt)
{

}
