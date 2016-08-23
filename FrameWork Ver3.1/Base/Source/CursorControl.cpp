#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"
#include "ArrowTower.h"
#include "CannonTower.h"
#include "PoisonTower.h"
#include "IceTower.h"
#include "BuffTower.h"
#include "CaptureTower.h"

CursorControl::CursorControl()
{
	bLButtonState = false;
	checkPositionX = 0;
	checkPositionY = 0;
}

CursorControl::~CursorControl()
{
	for (int i = 0; i < 4; ++i)
	{
		if (spawnTower[i] != nullptr)
		{
			delete spawnTower[i];
		}
		if (towerCosts[i] != nullptr)
		{
			delete towerCosts[i];
		}
	}

	if (towerName != nullptr)
	{
		delete towerName;
	}
	if (background != nullptr)
	{
		delete background;
	}
}

void CursorControl::Init(vector<Tower*> *towerList, vector<Enemy*> *enemyList)
{
	this->towerList = towerList;
	this->enemyList = enemyList;

	aoe.b_isActive = false;
	aoe.b_lightEnabled = false;
	aoe.meshID = GEO_RING;
	aoe.rotation.Set(90, 0, 0);
	aoe.b_shadows = false;

	towerName = new GUI("Name");
	towerName->SetTextSize(3);
	towerName->position.Set(40, 55);
	towerName->b_isActive = false;
	towerName->b_buttonActive = false;
	towerName->b_textActive = true;

	background = new GUI();
	background->b_isActive = false;
	background->b_lightEnabled = false;
	background->b_textActive = false;
	background->b_buttonActive = false;
	background->position.Set(40, 13);
	background->meshID = GEO_QUAD;
	background->scale.Set(70, 12, 1);

	TowerButtons();
}

static float debounce = 0.f;

void CursorControl::Update(OrthoCamera &camera, const TileMap &tileMap, const double &dt)
{
	Cursor::Update(camera, tileMap, dt);
	debounce += (float)dt;
	if (!bLButtonState) // If LClick is NOT being held down
	{
		checkPositionX = (int)Math::Clamp(worldCoords.x, 0.f, (float)tileMap.i_columns - 1.f);
		checkPositionY = (int)Math::Clamp(worldCoords.y, 0.f, (float)tileMap.i_rows - 1.f);

		camera.orthoSize = Math::Clamp(camera.orthoSize - (float)Application::mouse_scroll, 2.f, camera.defaultOrtho); // scrolling in and out
		EdgePanning(dt, camera, worldX, worldY, 6 * tileMap.i_rows);
	}
	else // If LClick is being held down
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
		towerName->SetText(tower->s_name);
		towerName->position.x = 40 - tower->s_name.size() / 1.5f;
		towerName->b_isActive = true;
	}
	else
	{
		towerName->b_isActive = false;
	}
	HotKeys(tileMap);
	AOEDisplay(tower);
	CameraBounds(camera);
	
	if (!bLButtonState && Application::IsMousePressed(0) && tileMap.screenMap[checkPositionX][checkPositionY] == -2) // -2 being a empty slot
	{
		bLButtonState = true;
		for (int i = 0; i < 4; ++i)
		{
			spawnTower[i]->b_isActive = true;
			towerCosts[i]->b_isActive = true;
		}
		background->b_isActive = true;
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
			else if (button->functionID == 2)
			{
				SpawnTower(string("Capture"));
				tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			}
			else if (button->functionID == 3)
			{
				SpawnTower(string("Buff"));
				tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			spawnTower[i]->b_isActive = false;
			towerCosts[i]->b_isActive = false;
		}
		background->b_isActive = false;
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
		tempTower = new PoisonTower();
	else if (name == string("Cannon"))
		tempTower = new CannonTower();
	else if (name == string("Capture"))
		tempTower = new CaptureTower();
	else if (name == string("Buff"))
		tempTower = new BuffTower();
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

void CursorControl::TowerButtons()
{
	for (int i = 0; i < 4; ++i)
	{
		string text;
		GEOMETRY_TYPE mesh;
		Vector2 offset;
		int cost = 0;
		int atk = 0;
		if (i == 0)
		{
			text = "Arrow Tower (Q)";
			mesh = GEO_ARROWTOWER;
			offset.Set(0.1f, 0.15f);
			
			cost = ArrowTower::cost;
		}
		else if (i == 1)
		{
			text = "Cannon Tower (W)";
			mesh = GEO_CANNONTOWER;
			offset.Set(0.3f, 0.15f);
			cost = CannonTower::cost;
		}
		else if (i == 2)
		{
			text = "Ice Tower (E)";
			mesh = GEO_ICETOWER;
			offset.Set(0.5f, 0.15f);
			cost = IceTower::cost;
		}
		else if (i == 3)
		{
			text = "Buff Tower (R)";
			mesh = GEO_ARROWTOWER;
			offset.Set(0.7f, 0.15f);
		}

		spawnTower[i] = new GUI(text);
		spawnTower[i]->meshID = mesh;
		spawnTower[i]->scale.Set(3, 3, 3);
		spawnTower[i]->rotation.Set(-90, 0, 0);
		spawnTower[i]->SetTextSize(2);
		spawnTower[i]->meshOffset.Set(5, 3, 50.f);
		spawnTower[i]->position.Set(offset.x * 80, offset.y * 60, 0.f);
		spawnTower[i]->buttonSize.Set(12, 17);
		spawnTower[i]->functionID = i;
		spawnTower[i]->b_isActive = false;

		std::ostringstream os;
		os << "Cost: " << cost;
		towerCosts[i] = new GUI(os.str());
		towerCosts[i]->SetParent(spawnTower[i]);
		towerCosts[i]->textColor.Set(1, 1, 0);
		towerCosts[i]->SetTextSize(2);
		towerCosts[i]->b_buttonActive = false;
		towerCosts[i]->position.Set(0, -1.5f);
		towerCosts[i]->b_isActive = false;
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

void CursorControl::HotKeys(const TileMap &tileMap)
{
	float cooldown = 0.5f;

	if (Application::IsKeyPressed('Q'))
	{
		if (bLButtonState && debounce > cooldown)
		{
			debounce = 0.f;
			SpawnTower("Arrow");
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			bLButtonState = false;

			for (int i = 0; i < 4; ++i)
			{
				spawnTower[i]->b_isActive = false;
				towerCosts[i]->b_isActive = false;
			}
			background->b_isActive = false;
		}
	}
	else if (Application::IsKeyPressed('W'))
	{
		if (bLButtonState && debounce > cooldown)
		{
			debounce = 0.f;
			SpawnTower("Cannon");
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			bLButtonState = false;

			for (int i = 0; i < 4; ++i)
			{
				spawnTower[i]->b_isActive = false;
				towerCosts[i]->b_isActive = false;
			}
			background->b_isActive = false;
		}
	}
	else if (Application::IsKeyPressed('E'))
	{
		if (bLButtonState && debounce > cooldown)
		{
			debounce = 0.f;
			SpawnTower("Capture");
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			bLButtonState = false;

			for (int i = 0; i < 4; ++i)
			{
				spawnTower[i]->b_isActive = false;
				towerCosts[i]->b_isActive = false;
			}
			background->b_isActive = false;
		}
	}
	else if (Application::IsKeyPressed('R'))
	{
		if (bLButtonState && debounce > cooldown)
		{
			debounce = 0.f;
			SpawnTower("Buff");
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			bLButtonState = false;

			for (int i = 0; i < 4; ++i)
			{
				spawnTower[i]->b_isActive = false;
				towerCosts[i]->b_isActive = false;
			}
			background->b_isActive = false;
		}
	}
}