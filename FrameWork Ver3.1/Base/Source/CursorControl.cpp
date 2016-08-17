#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"
#include "ArrowTower.h"

CursorControl::CursorControl()
{
	checkPositionX = 0;
	checkPositionY = 0;
}

CursorControl::~CursorControl()
{

}

void CursorControl::Init(vector<Tower*> *towerList, vector<Enemy*> *enemyList)
{
	this->towerList = towerList;
	this->enemyList = enemyList;
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
		if (tileMap.screenMap[checkPositionX][checkPositionY] == -1)
		{
			SpawnTower();
			tileMap.screenMap[checkPositionX][checkPositionY] = -2;
		}

	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
	if (!bLButtonState && Application::IsMousePressed(1))
	{
		bLButtonState = true;
		if (tileMap.screenMap[checkPositionX][checkPositionY] == -1)
		{
			SpawnTower();
			tileMap.screenMap[checkPositionX][checkPositionY] = -2;
		}

	}
	else if (bLButtonState && !Application::IsMousePressed(1))
	{
		bLButtonState = false;
	}
}

bool CursorControl::SpawnTower()
{
	Tower *tempTower = new ArrowTower();
	tempTower->pos.Set(checkPositionX, checkPositionY, 0);
	tempTower->scale.Set(1, 1, 1);
	tempTower->enemyList = this->enemyList;
	towerList->push_back(tempTower);
	return true;
}