#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"
#include "ArrowTower.h"
#include "CannonTower.h"

CursorControl::CursorControl()
{
	checkPositionX = 0;
	checkPositionY = 0;

	for (int i = 0; i < 4; ++i)
	{
		spawnTower[i] = nullptr;
	}
}

CursorControl::~CursorControl()
{

}

void CursorControl::Init(vector<Tower*> *towerList, vector<Enemy*> *enemyList)
{
	this->towerList = towerList;
	this->enemyList = enemyList;
}

void CursorControl::Update(OrthoCamera &camera, const TileMap &tileMap, const double &dt)
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

	static bool bLButtonState = false;

	if (!bLButtonState)
	{
		checkPositionX = (int)Math::Clamp(worldCoords.x, 0.f, (float)tileMap.i_columns - 1.f);
		checkPositionY = (int)Math::Clamp(worldCoords.y, 0.f, (float)tileMap.i_rows - 1.f);

		camera.orthoSize = Math::Clamp(camera.orthoSize - (float)Application::mouse_scroll, 3.f, camera.defaultOrtho);

		EdgePanning(dt, camera, worldX, worldY);
	}
	
	
	if (!bLButtonState && Application::IsMousePressed(0) && tileMap.screenMap[checkPositionX][checkPositionY] == -2)
	{
		bLButtonState = true;
		TowerButtons(worldX, worldY);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;

		GUI* button = GUIManager::GetInstance()->FindGUI(worldX, worldY);

		if (button != nullptr)
		{
			if (button->functionID == 0)
			{
				SpawnTower(string("Arrow"));
				tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			}
			else if (button->functionID == 1)
			{
				SpawnTower(string("Cannon"));
				tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			if (spawnTower[i] != nullptr)
			{
				delete spawnTower[i];
			}
		}
	}

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
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

void CursorControl::TowerButtons(float worldX,float worldY)
{
	spawnTower[0] = new GUI("Arrow Tower");
	spawnTower[0]->meshID = GEO_ARROWTOWER;
	spawnTower[0]->scale.Set(3, 3, 3);
	spawnTower[0]->rotation.Set(-90, 0, 0);
	spawnTower[0]->SetTextSize(2);
	spawnTower[0]->meshOffset.Set(5, 3, 0);
	spawnTower[0]->position.Set((worldX + 0.5f) * 80 - 20.f, (worldY + 0.5f) * 60 - 15.f);
	spawnTower[0]->buttonSize.Set(17, 17);
	spawnTower[0]->functionID = 0;

	spawnTower[1] = new GUI("Mortar Tower");
	spawnTower[1]->meshID = GEO_CANNONTOWER;
	spawnTower[1]->scale.Set(3, 3, 3);
	spawnTower[1]->rotation.Set(-90, 0, 0);
	spawnTower[1]->SetTextSize(2);
	spawnTower[1]->meshOffset.Set(5, 3, 0);
	spawnTower[1]->position.Set((worldX + 0.5f) * 80 + 5.f, (worldY + 0.5f) * 60 - 15.f);
	spawnTower[1]->buttonSize.Set(17, 17);
	spawnTower[1]->functionID = 1;

	spawnTower[2] = new GUI("Some other Tower");
	spawnTower[2]->meshID = GEO_ARROWTOWER;
	spawnTower[2]->scale.Set(3, 3, 3);
	spawnTower[2]->rotation.Set(-90, 0, 0);
	spawnTower[2]->SetTextSize(2);
	spawnTower[2]->meshOffset.Set(5, 3, 0);
	spawnTower[2]->position.Set((worldX + 0.5f) * 80 + 5.f, (worldY + 0.5f) * 60 + 5.f);
	spawnTower[2]->buttonSize.Set(17, 17);
	spawnTower[2]->functionID = 2;

	spawnTower[3] = new GUI("Another Tower");
	spawnTower[3]->meshID = GEO_ARROWTOWER;
	spawnTower[3]->scale.Set(3, 3, 3);
	spawnTower[3]->rotation.Set(-90, 0, 0);
	spawnTower[3]->SetTextSize(2);
	spawnTower[3]->meshOffset.Set(5, 3, 0);
	spawnTower[3]->position.Set((worldX + 0.5f) * 80 - 20.f, (worldY + 0.5f) * 60 + 5.f);
	spawnTower[3]->buttonSize.Set(17, 17);
	spawnTower[3]->functionID = 3;
}

void CursorControl::EdgePanning(const double &dt, OrthoCamera &camera, float worldX, float worldY)
{
	float speed = 60;

	if (worldX > 0.4)
	{
		float offset = Math::Clamp(camera.target.x + (worldX - 0.4f) * speed * (float)dt, camera.defaultTarget.x - camera.defaultOrtho * (float)(camera.aspectRatio.x / camera.aspectRatio.y), camera.defaultTarget.x + camera.defaultOrtho * (float)(camera.aspectRatio.x / camera.aspectRatio.y));
		offset = offset - camera.target.x;
		camera.target.x += offset;
		camera.position.x += offset;

	}
	else if (worldX < -0.4)
	{
		float offset = Math::Clamp(camera.target.x + (worldX + 0.4f) * speed * (float)dt, camera.defaultTarget.x - camera.defaultOrtho * (float)(camera.aspectRatio.x / camera.aspectRatio.y), camera.defaultTarget.x + camera.defaultOrtho * (float)(camera.aspectRatio.x / camera.aspectRatio.y));
		offset = offset - camera.target.x;
		camera.target.x += offset;
		camera.position.x += offset;
	}

	if (worldY > 0.4)
	{
		float offset = Math::Clamp(camera.target.y + (worldY - 0.4f) * speed * (float)dt, camera.defaultTarget.y - camera.defaultOrtho, camera.defaultTarget.y + camera.defaultOrtho);
		offset = offset - camera.target.y;
		camera.position.y += offset;
		camera.target.y += offset;
	}
	else if (worldY < -0.4)
	{
		float offset = Math::Clamp(camera.target.y + (worldY + 0.4f) * speed * (float)dt, camera.defaultTarget.y - camera.defaultOrtho, camera.defaultTarget.y + camera.defaultOrtho);
		offset = offset - camera.target.y;
		camera.position.y += offset;
		camera.target.y += offset;
	}
}