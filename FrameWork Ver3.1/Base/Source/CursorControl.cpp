#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"

CursorControl::CursorControl()
{
	checkPositionX = 0;
	checkPositionY = 0;
}

CursorControl::~CursorControl()
{

}

void CursorControl::Update(const Camera &camera, const TileMap &tileMap)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	float worldX = x / w * 100;
	float worldY = (h - y) / h * 100 * (camera.aspectRatio.y / camera.aspectRatio.x);

	float sensitivity = 60.f;

	this->checkPositionX = Math::Clamp((worldX - 15.f) / (sensitivity / (tileMap.i_columns - 1)), 0.f, (float)tileMap.i_columns - 1);
	this->checkPositionY = Math::Clamp((worldY - 2.75f) / (sensitivity / (tileMap.i_rows - 1)), 0.f, (float)tileMap.i_rows - 1);


	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
}