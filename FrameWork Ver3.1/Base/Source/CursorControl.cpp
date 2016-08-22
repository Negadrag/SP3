#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"
#include "ArrowTower.h"
#include "CannonTower.h"
#include "IceTower.h"
#include "PoisonTower.h"

CursorControl::CursorControl()
{

	bLButtonState = false;
	checkPositionX = 0;
	checkPositionY = 0;

	aoe.b_isActive = false;
	aoe.b_lightEnabled = false;
	aoe.meshID = GEO_RING;
	aoe.rotation.Set(90, 0, 0);
	aoe.b_shadows = false;

	towerName.SetText("Name");
	towerName.SetTextSize(3);
	towerName.position.Set(40,55);
	towerName.b_isActive = false;
	towerName.b_buttonActive = false;
	towerName.b_textActive = true;
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

	if (!bLButtonState) // If LClick is being held down
	{
		checkPositionX = (int)Math::Clamp(worldCoords.x, 0.f, (float)tileMap.i_columns - 1.f);
		checkPositionY = (int)Math::Clamp(worldCoords.y, 0.f, (float)tileMap.i_rows - 1.f);

		camera.orthoSize = Math::Clamp(camera.orthoSize - (float)Application::mouse_scroll, 2.f, camera.defaultOrtho); // scrolling in and out
		EdgePanning(dt, camera, worldX, worldY, 6 * tileMap.i_rows);
	}
	else // If LClick is not being held down
	{
		GUI* button = GUIManager::GetInstance()->FindGUI(worldX, worldY);
		if (button != nullptr)
		{
			button->rotation.y = Math::Wrap(button->rotation.y + 50.f * (float)dt,0.f,360.f);
		}
	}


	Tower* tower = FindTower(checkPositionX, checkPositionY);
	if (tower != nullptr)
	{
		towerName.SetText(tower->s_name);
		towerName.position.x = 40 - tower->s_name.size() / 1.5f;
		towerName.b_isActive = true;
	}
	else
	{
		towerName.b_isActive = false;
	}

	AOEDisplay(tower);


	CameraBounds(camera);
	
	if (!bLButtonState && Application::IsMousePressed(0) && tileMap.screenMap[checkPositionX][checkPositionY] == -2) // -2 being a empty slot
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
				//SpawnTower(string("Arrow"));
				SpawnTower(string("Poison"));
				tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			}
			else if (button->functionID == 1)
			{
				SpawnTower(string("Cannon"));
				tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			}
			else if (button->functionID == 2)
			{
				SpawnTower(string("Ice"));
				tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			spawnTower[i].b_isActive = false;
			towerCosts[i].b_isActive = false;
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
	/*if (name == string("Arrow"))
		tempTower = new ArrowTower();*/
	if (name == string("Poison"))
		tempTower = new PoisonTower();
	else if (name == string("Cannon"))
		tempTower = new CannonTower();
	else if (name == string("Ice"))
		tempTower = new IceTower();
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
	for (int i = 0; i < 4; ++i)
	{
		string text;
		GEOMETRY_TYPE mesh;
		Vector2 offset;
		int cost = 0;
		if (i == 0)
		{
			text = "Arrow Tower";
			//mesh = GEO_ARROWTOWER;
			mesh = GEO_POISONTOWER;
			offset.Set(-15.f, -15.f);
			//cost = ArrowTower::cost;
		}
		else if (i == 1)
		{
			text = "Cannon Tower";
			mesh = GEO_CANNONTOWER;
			offset.Set(5.f, -15.f);
			cost = CannonTower::cost;
		}
		else if (i == 2)
		{
			text = "Ice Tower";
			mesh = GEO_ICETOWER;
			offset.Set(5.f, 5.f);
			cost = IceTower::cost;
		}
		else if (i == 3)
		{
			text = "Another One";
			mesh = GEO_ARROWTOWER;
			offset.Set(-15.f, 5.f);
		}

		spawnTower[i].SetText(text);
		spawnTower[i].meshID = mesh;
		spawnTower[i].scale.Set(3, 3, 3);
		spawnTower[i].rotation.Set(-90, 0, 0);
		spawnTower[i].SetTextSize(2);
		spawnTower[i].meshOffset.Set(5, 3, 0);
		spawnTower[i].position.Set((worldX + 0.5f) * 80 + offset.x, (worldY + 0.5f) * 60 + offset.y);
		spawnTower[i].buttonSize.Set(12, 17);
		spawnTower[i].functionID = i;
		spawnTower[i].b_isActive = true;

		std::ostringstream os;
		os << "Cost: " << cost;
		towerCosts[i].SetText(os.str());
		towerCosts[i].SetParent(&spawnTower[i]);
		towerCosts[i].textColor.Set(1, 1, 0);
		towerCosts[i].SetTextSize(2);
		towerCosts[i].b_buttonActive = false;
		towerCosts[i].position.Set(0, -2);
		towerCosts[i].b_isActive = true;
	}
}

void CursorControl::EdgePanning(const double &dt, OrthoCamera &camera, float worldX, float worldY, float speed)
{
	if (worldX > 0.4)
	{
		camera.target.x += (worldX - 0.4f) * speed * (float)dt;
		camera.position.x += (worldX - 0.4f) * speed * (float)dt;
	}
	if (worldX < -0.4)
	{
		camera.target.x += (worldX + 0.4f) * speed * (float)dt;
		camera.position.x += (worldX + 0.4f) * speed * (float)dt;
	}

	if (worldY > 0.4)
	{
		camera.target.y += (worldY - 0.4f) * speed * (float)dt;
		camera.position.y += (worldY - 0.4f) * speed * (float)dt;
	}
	if (worldY < -0.4)
	{
		camera.target.y += (worldY + 0.4f) * speed * (float)dt;
		camera.position.y += (worldY + 0.4f) * speed * (float)dt;
	}
}

void CursorControl::CameraBounds(OrthoCamera &camera)
{
	if (camera.target.x > camera.defaultTarget.x + (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y))
	{
		float offset = camera.defaultTarget.x + (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y) - camera.target.x;
		camera.target.x += offset;
		camera.position.x += offset;
	}
	else if (camera.target.x < camera.defaultTarget.x - (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y))
	{
		float offset = camera.defaultTarget.x - (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y) - camera.target.x;
		camera.target.x += offset;
		camera.position.x += offset;
	}

	if (camera.target.y > camera.defaultTarget.y + (camera.defaultOrtho - camera.orthoSize))
	{
		float offset = camera.defaultTarget.y + (camera.defaultOrtho - camera.orthoSize) - camera.target.y;
		camera.target.y += offset;
		camera.position.y += offset;
	}
	else if (camera.target.y < camera.defaultTarget.y - (camera.defaultOrtho - camera.orthoSize))
	{
		float offset = camera.defaultTarget.y - (camera.defaultOrtho - camera.orthoSize) - camera.target.y;
		camera.target.y += offset;
		camera.position.y += offset;
	}
}

void CursorControl::AOEDisplay(Tower* tower)
{
	if (tower != nullptr)
	{
		aoe.b_isActive = true;
		aoe.pos.Set(tower->pos.x, tower->pos.y, 0.5f);
		aoe.scale.Set(tower->GetRange(), tower->GetRange(), tower->GetRange());
	}
	else
	{
		aoe.b_isActive = false;
	}
}