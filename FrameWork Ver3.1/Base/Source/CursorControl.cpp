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

void CursorControl::Update(const OrthoCamera &camera, const TileMap &tileMap)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	float worldX = x / w - 0.5f;
	float worldY = 1.f - (y / h) - 0.5f;

	worldY = worldY / cos(Math::DegreeToRadian(camera.rotation));

	float Xunits = 0.5f/ (camera.orthoSize * (camera.aspectRatio.x / camera.aspectRatio.y));
	float Yunits = 0.5f / (camera.orthoSize);

	Vector3 center = camera.target;

	std::cout << worldY << std::endl;

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
			SpawnTower();
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
			SpawnTower();
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
		}

	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
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
