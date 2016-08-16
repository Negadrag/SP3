#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"

CursorControl::CursorControl()
{
	x = 0;
	y = 0;
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
	float worldY = (h - y) / h * 100 * (camera.aspectRatio.y / camera.aspectRatio.x); // 0.05 = 1 unit

	this->x = Math::Clamp((worldX - 15.f) / 5.35f, 0.f, (float)tileMap.i_columns - 1);
	this->y = Math::Clamp((worldY - 2.75f) / 5.25f, 0.f, (float)tileMap.i_rows - 1);
}