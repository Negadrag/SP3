#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"
#include "ArrowTower.h"
#include "CannonTower.h"

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

void CursorControl::Update(const OrthoCamera &camera, const TileMap &tileMap)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	float worldX = x / w - 0.5f; // -0.5 - 0.5
	float worldY = 1.f - (y / h) - 0.5f; // -0.5 - 0.5

	worldY = worldY / cos(Math::DegreeToRadian(camera.rotation)); // Slanting the camera by getting the hypotenuse

	float Xunits = 0.5f/ (camera.orthoSize * (camera.aspectRatio.x / camera.aspectRatio.y)); // 1 unit in world space
	float Yunits = 0.5f / (camera.orthoSize);

	Vector3 center = camera.target;

	worldCoords.Set(center.x + worldX/Xunits + 0.5f,center.y + worldY/Yunits + 0.5f);

	//std::cout << worldCoords << std::endl;

	checkPositionX = (int)Math::Clamp(worldCoords.x,0.f,(float)tileMap.i_columns - 1.f);
	checkPositionY = (int)Math::Clamp(worldCoords.y, 0.f, (float)tileMap.i_rows - 1.f);
	
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		if (tileMap.screenMap[checkPositionX][checkPositionY] == -2)
		{
			SpawnTower(string("Arrow"));
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
		}

	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		if (tileMap.screenMap[checkPositionX][checkPositionY] == -2)
		{
			SpawnTower(string("Cannon"));
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
		}

	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
	}
}

bool CursorControl::SpawnTower(string name)
{
	Tower *tempTower;
	if (name == string("Arrow"))
		tempTower = new ArrowTower();
	else if (name == string("Cannon"))
		tempTower = new CannonTower();
	tempTower->pos.Set(checkPositionX, checkPositionY, 0);
	tempTower->scale.Set(1, 1, 1);
	tempTower->enemyList = enemyList;
	towerList->push_back(tempTower);
	return true;
}

Tower* CursorControl::FindTower(int x, int y)
{
	for (vector<Tower*>::iterator it = towerList->begin(); it != towerList->end(); ++it)
	{
		Tower* tower = *it;
		if (tower->pos.x == x && tower->pos.y == y)
		{
			return tower;
		}
	}
	return nullptr;
}