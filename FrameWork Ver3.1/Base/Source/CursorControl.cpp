#include "CursorControl.h"
#include "Application.h"
#include "MathUtility.h"

#include "Scene.h"
#include "ArrowTower.h"
#include "CannonTower.h"
#include "PoisonTower.h"
#include "SpeedTower.h"
#include "IceTower.h"
#include "BuffTower.h"
#include "MortarTower.h"
#include "CaptureTower.h"

CursorControl::CursorControl()
{
	bLButtonState = false;
	bPlacingTower = false;
	checkPositionX = 0;
	checkPositionY = 0;
}

CursorControl::~CursorControl()
{
	Clear();
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
	background->scale.Set(70, 15, 1);

	TowerButtons();

	background2 = new GUI();
	background2->b_isActive = false;
	background2->b_lightEnabled = false;
	background2->b_textActive = false;
	background2->b_buttonActive = false;
	background2->position.Set(15, 0);
	background2->meshID = GEO_QUAD;
	background2->scale.Set(40, 12, 1);
	background2->SetParent(towerStats[0]);

	background3 = new GUI();
	background3->b_isActive = false;
	background3->b_lightEnabled = false;
	background3->b_textActive = false;
	background3->b_buttonActive = false;
	background3->position.Set(7, -10);
	background3->meshID = GEO_QUAD;
	background3->scale.Set(18, 50, 1);
	background3->SetParent(towerUpgrades[0]);

	selling = new GUI("(f) Sell:");
	selling->b_isActive = false;
	selling->SetTextSize(2);
	selling->textColor.Set(1, 0, 0);
	selling->SetParent(towerStats[0]);
	selling->position.Set(0, -5.5f, 0);

	skip = new GUI("Skip");
	skip->b_isActive = true;
	skip->b_lightEnabled = false;
	skip->SetTextSize(2);
	skip->textColor.Set(1, 1, 0);
	skip->position.Set(70, 5, 5);
	skip->buttonSize.Set(10, 10);
	skip->meshID = GEO_QUAD;
	skip->scale.Set(10, 10, 1);
	skip->meshOffset.Set(5, 2.5f, 0);
	skip->textOffset.Set(2.5, 1.5, 0);
	skip->b_buttonActive = true;
	skip->functionID = 10;
}

static float debounce = 0.f;

void CursorControl::Update(OrthoCamera &camera, TileMap &tileMap, const double &dt)
{
	Cursor::Update(camera, tileMap, dt);
	debounce += (float)dt;
	if (!bPlacingTower) // If LClick is NOT being held down
	{
		checkPositionX = (int)Math::Clamp(worldCoords.x, 0.f, (float)tileMap.i_columns - 1.f);
		checkPositionY = (int)Math::Clamp(worldCoords.y, 0.f, (float)tileMap.i_rows - 1.f);

		camera.orthoSize = Math::Clamp(camera.orthoSize - (float)Application::mouse_scroll, 2.f, camera.defaultOrtho); // scrolling in and out
		EdgePanning(dt, camera, screenX, screenY, 6 * tileMap.i_rows);
	}
	else // If LClick is being held down
	{
		GUI* button = GUIManager::GetInstance()->FindGUI(screenX, screenY);
		if (button != nullptr)
		{
			button->rotation.y = Math::Wrap(button->rotation.y + 50.f * (float)dt,0.f,360.f);
		}
	}

	Tower* tower = FindTower(checkPositionX, checkPositionY);
	if (tower != nullptr)
	{
		std::ostringstream os;
		if (tower->i_level > 0)
		{
			os << tower->s_name << " Lvl." << tower->i_level;
		}
		else
		{
			os << tower->s_name;
		}
		
		towerName->SetText(os.str());
		towerName->position.x = 40 - tower->s_name.size() / 1.5f;
		towerName->b_isActive = true;

		os.str("");
		os << "Atk: " << tower->GetAtkDmg();
		towerStats[T_ATK]->SetText(os.str());
		towerStats[T_ATK]->b_isActive = true;

		os.str("");
		os << "Speed: " << tower->GetSpdRate();
		towerStats[T_SPEED]->SetText(os.str());
		towerStats[T_SPEED]->b_isActive = true;

		os.str("");
		os << "Strategy: " << Tower::StrategyToString(tower->strategy) << " (R-Click)";
		towerStats[T_STRATEGY]->SetText(os.str());
		towerStats[T_STRATEGY]->b_isActive = true;

		os.str("");
		os << "(F) Sell :" << (int)tower->GetCost() / 2;
		selling->SetText(os.str());
		selling->b_isActive = true;

		background2->b_isActive = true;
	}
	else
	{
		towerName->b_isActive = false;
		selling->b_isActive = false;
		background2->b_isActive = false;
		for (int i = 0; i < (int)T_TOTAL; ++i)
		{
			towerStats[i]->b_isActive = false;
		}
	}

	if (!tileMap.waves.b_waveEnded)
	{
		skip->b_isActive = false;
	}
	else
	{
		skip->b_isActive = true;
	}

	if (GUIManager::GetInstance()->FindGUI(screenX, screenY))
	{
		if (GUIManager::GetInstance()->FindGUI(screenX, screenY)->functionID == 10)
		{
			GUIManager::GetInstance()->FindGUI(screenX, screenY)->rotation.Set(10, 0, 0);
		}
	}
	else
	{
		skip->rotation.Set(0, 0, 0);
	}

	AOEDisplay(tower);
	Clicking(tileMap);
	CameraBounds(camera);
	HotKeys(tileMap);
}

bool CursorControl::SpawnTower(string name)
{
	Tower *tempTower;
	if (name == string("Arrow"))
		tempTower = new ArrowTower();
	else if (name == string("Cannon"))
		tempTower = new CannonTower();
	else if (name == string("Capture"))
		tempTower = new CaptureTower();
	else if (name == string("Buff"))
	{
		tempTower = new BuffTower(towerList);
	}
	else if (name == string("Ice"))
		tempTower = new IceTower();
	else if (name == string("Poison"))
		tempTower = new PoisonTower();
	else if (name == string("Mortar"))
		tempTower = new MortarTower();
	else if (name == string("Speed"))
		tempTower = new SpeedTower();
	else
		tempTower = nullptr;
	if (tempTower != nullptr)
	{
		tempTower->pos.Set(checkPositionX, checkPositionY, 0);
		tempTower->scale.Set(1, 1, 1);
		tempTower->enemyList = enemyList;
		towerList->push_back(tempTower);
		return true;
	}
	else
	{
		return false;
	}
}

bool CursorControl::RemoveTower(Tower* tower)
{
	if (towerList != nullptr)
	{
		for (vector<Tower*>::iterator it = towerList->begin(); it != towerList->end(); ++it)
		{
			if ((*it)->s_name == "Buff Tower")
			{
				BuffTower *tower2 = dynamic_cast<BuffTower*>(*it);
				tower2->RemoveTower(tower);
			}
		}
		for (vector<Tower*>::iterator it = towerList->begin(); it != towerList->end(); ++it)
		{
			if (tower == *it)
			{
				delete *it;
				it = towerList->erase(it);
				return true;
			}
		}
	}
	return false;
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
			text = "Arrow Tower(Q)";
			mesh = GEO_ARROWTOWER;
			offset.Set(0.1f, 0.15f);
			
			cost = ArrowTower::cost;
		}
		else if (i == 1)
		{
			text = "Cannon Tower(W)";
			mesh = GEO_CANNONTOWER;
			offset.Set(0.3f, 0.15f);
			cost = CannonTower::cost;
		}
		else if (i == 2)
		{
			text = "Capture Tower(E)";
			mesh = GEO_CAPTURETOWER;
			offset.Set(0.5f, 0.15f);
			cost = CaptureTower::cost;
		}
		else if (i == 3)
		{
			text = "Buff Tower(R)";
			mesh = GEO_BUFFTOWER;
			offset.Set(0.7f, 0.15f);
			cost = BuffTower::cost;
		}

		spawnTower[i] = new GUI(text);
		spawnTower[i]->meshID = mesh;
		spawnTower[i]->scale.Set(3, 3, 3);
		spawnTower[i]->rotation.Set(-90, 0, 0);
		spawnTower[i]->SetTextSize(2);
		spawnTower[i]->meshOffset.Set(5, 3, 50.f);
		spawnTower[i]->position.Set(offset.x * 80, offset.y * 60, 0.f);
		spawnTower[i]->buttonSize.Set(12, 10);
		spawnTower[i]->functionID = i;
		spawnTower[i]->b_isActive = false;

		std::ostringstream os;
		os << "  " << cost;
		towerCosts[i] = new GUI(os.str());
		towerCosts[i]->meshID = GEO_COIN;
		towerCosts[i]->scale.Set(1.f, 1.f, 1.f);
		towerCosts[i]->meshOffset.Set(1, 1, 1);
		towerCosts[i]->SetParent(spawnTower[i]);
		towerCosts[i]->textColor.Set(1, 1, 0);
		towerCosts[i]->SetTextSize(2);
		towerCosts[i]->b_buttonActive = false;
		towerCosts[i]->position.Set(0, -1.5f);
		towerCosts[i]->b_isActive = false;
		towerCosts[i]->b_lightEnabled = false;
	}

	for (int i = 0; i < (int)T_TOTAL; ++i)
	{
		towerStats[i] = new GUI();
		towerStats[i]->textColor.Set(1, 1, 1);
		towerStats[i]->SetTextSize(2);
		towerStats[i]->b_buttonActive = false;
		towerStats[i]->position.Set(27.f, 52.f - i * 2.f);
		towerStats[i]->b_isActive = false;
	}

	for (int i = 0; i < 3; ++i)
	{
		towerUpgrades[i] = new GUI("Hello");
		towerUpgrades[i]->scale.Set(3, 3, 3);
		towerUpgrades[i]->rotation.Set(-90, 0, 0);
		towerUpgrades[i]->SetTextSize(2);
		towerUpgrades[i]->meshOffset.Set(5, 3, 50.f);
		towerUpgrades[i]->position.Set(5.f, 40 - i * 15.f, 0.f);
		towerUpgrades[i]->buttonSize.Set(10, 10);
		towerUpgrades[i]->b_isActive = false;

		upgradeCosts[i] = new GUI("cost");
		upgradeCosts[i]->SetParent(towerUpgrades[i]);
		upgradeCosts[i]->textColor.Set(1, 1, 0);
		upgradeCosts[i]->SetTextSize(2);
		upgradeCosts[i]->b_buttonActive = false;
		upgradeCosts[i]->position.Set(0, -1.5f);
		upgradeCosts[i]->b_isActive = false;

		essenceCost[i] = new GUI("essencecost");
		essenceCost[i]->SetParent(towerUpgrades[i]);
		essenceCost[i]->textColor.Set(1, 1, 0);
		essenceCost[i]->SetTextSize(2);
		essenceCost[i]->b_buttonActive = false;
		essenceCost[i]->position.Set(0, -3.0f);
		essenceCost[i]->b_isActive = false;
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

void CursorControl::HotKeys(TileMap &tileMap)
{
	float cooldown = 0.5f;

	if (Application::IsKeyPressed('Q'))
	{
		if (bLButtonState && FindTower(checkPositionX,checkPositionY) == nullptr && tileMap.screenMap[checkPositionX][checkPositionY] == -2 && debounce > cooldown)
		{
			debounce = 0.f;
			if (Scene::player.i_currency >= ArrowTower::cost)
			{
				Scene::player.i_currency -= ArrowTower::cost;
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
		else if (bPlacingTower && FindTower(checkPositionX, checkPositionY) != nullptr && debounce > cooldown)
		{
			debounce = 0.f;
			HandleButton(tileMap, towerUpgrades[0]);
			for (int i = 0; i < 3; ++i)
			{
				towerUpgrades[i]->b_isActive = false;
				upgradeCosts[i]->b_isActive = false;
				essenceCost[i]->b_isActive = false;
			}
			bLButtonState = false;
			bPlacingTower = false;
			background3->b_isActive = false;
		}
	}
	else if (Application::IsKeyPressed('W'))
	{
		if (bLButtonState && FindTower(checkPositionX, checkPositionY) == nullptr && tileMap.screenMap[checkPositionX][checkPositionY] == -2 && debounce > cooldown)
		{
			debounce = 0.f;
			if (Scene::player.i_currency >= CannonTower::cost)
			{
				Scene::player.i_currency -= CannonTower::cost;
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
		else if (bPlacingTower && FindTower(checkPositionX, checkPositionY) != nullptr && debounce > cooldown)
		{
			debounce = 0.f;
			if (towerUpgrades[1]->b_isActive)
			{
				HandleButton(tileMap, towerUpgrades[1]);
				for (int i = 0; i < 3; ++i)
				{
					towerUpgrades[i]->b_isActive = false;
					upgradeCosts[i]->b_isActive = false;
					essenceCost[i]->b_isActive = false;
				}
				bLButtonState = false;
				bPlacingTower = false;
				background3->b_isActive = false;
			}
		}
	}
	else if (Application::IsKeyPressed('E'))
	{
		if (bLButtonState && FindTower(checkPositionX, checkPositionY) == nullptr && tileMap.screenMap[checkPositionX][checkPositionY] == -2 && debounce > cooldown)
		{
			debounce = 0.f;
			if (Scene::player.i_currency >= CaptureTower::cost)
			{
				Scene::player.i_currency -= CaptureTower::cost;
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
		else if (bPlacingTower && FindTower(checkPositionX, checkPositionY) != nullptr && debounce > cooldown)
		{
			debounce = 0.f;
			if (towerUpgrades[2]->b_isActive)
			{
				HandleButton(tileMap, towerUpgrades[2]);
				for (int i = 0; i < 3; ++i)
				{
					towerUpgrades[i]->b_isActive = false;
					upgradeCosts[i]->b_isActive = false;
					essenceCost[i]->b_isActive = false;
				}
				bLButtonState = false;
				bPlacingTower = false;
				background3->b_isActive = false;
			}
		}
	}
	else if (Application::IsKeyPressed('R'))
	{
		if (bLButtonState && FindTower(checkPositionX, checkPositionY) == nullptr && debounce > cooldown)
		{
			debounce = 0.f;
			if (Scene::player.i_currency >= BuffTower::cost)
			{
				Scene::player.i_currency -= BuffTower::cost;
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
	if (Application::IsKeyPressed('F'))
	{
		if (FindTower(checkPositionX, checkPositionY) != nullptr && tileMap.screenMap[checkPositionX][checkPositionY] == -3)
		{
			Tower *tower = FindTower(checkPositionX, checkPositionY);
			Scene::player.i_currency += (int)tower->GetCost() / 2;
			if (RemoveTower(tower))
			{
				tileMap.screenMap[checkPositionX][checkPositionY] = -2;
			}
		}
	}
}

void CursorControl::Clicking(TileMap &tileMap)
{
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		if (tileMap.screenMap[checkPositionX][checkPositionY] == -2) // -2 being a tower slot
		{
			bPlacingTower = true;
			for (int i = 0; i < 4; ++i)
			{
				spawnTower[i]->b_isActive = true;
				towerCosts[i]->b_isActive = true;
			}
			background->b_isActive = true;
		}
		else if (FindTower(checkPositionX, checkPositionY) != nullptr)
		{
			bPlacingTower = true;
			Tower* temp = FindTower(checkPositionX, checkPositionY);
			if (temp->i_level != 0)
			{
				UpgradeButtons(temp);
			}
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		bPlacingTower = false;
		GUI* button = GUIManager::GetInstance()->FindGUI(screenX, screenY);
		
		if (button != nullptr)
		{
			HandleButton(tileMap, button);
		}

		for (int i = 0; i < 4; ++i)
		{
			spawnTower[i]->b_isActive = false;
			towerCosts[i]->b_isActive = false;
		}

		for (int i = 0; i < 3; ++i)
		{
			towerUpgrades[i]->b_isActive = false;
			upgradeCosts[i]->b_isActive = false;
			essenceCost[i]->b_isActive = false;
		}

		background->b_isActive = false;
		background3->b_isActive = false;
	}

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		Tower* tower = FindTower(checkPositionX, checkPositionY);
		if (tower != nullptr)
		{
			tower->strategy = static_cast<Tower::STRATEGY>(tower->strategy + 1);
			if (tower->strategy == Tower::NUM_STRATEGY)
			{
				tower->strategy = static_cast<Tower::STRATEGY>(0);
			}
		}
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
	}
}

void CursorControl::UpgradeButtons(Tower* tower)
{
	if (tower->i_level < tower->i_MaxLevel)
	{
		background3->b_isActive = true;

		towerUpgrades[0]->SetText("Level up (Q)");
		towerUpgrades[0]->b_isActive = true;
		towerUpgrades[0]->meshID = tower->fullMeshID;
		towerUpgrades[0]->functionID = 5;
		std::ostringstream os;
		os << "  " << tower->GetCost();
		upgradeCosts[0]->meshID = GEO_COIN;
		upgradeCosts[0]->scale.Set(1.f, 1.f, 1.f);
		upgradeCosts[0]->meshOffset.Set(1, 1, 0);
		upgradeCosts[0]->SetText(os.str());
		upgradeCosts[0]->b_isActive = true;
		upgradeCosts[0]->b_lightEnabled = false;
		if (tower->essenceUpgradeCost > 0)
		{
			os.str("");
			os << "  " << tower->essenceUpgradeCost;
			essenceCost[0]->scale.Set(1.5f, 1.5f, 1.5f);
			essenceCost[0]->meshOffset.Set(1, 1, 0);
			essenceCost[0]->SetText(os.str());
			CostColor(essenceCost[0], tower->essence);
			essenceCost[0]->b_isActive = true;
			essenceCost[0]->b_lightEnabled = false;
		}
	}

	if (tower->i_level == 1)
	{
		if (tower->upgrades[0] != "")
		{
			SetUpgradeButtons(towerUpgrades[1], upgradeCosts[1], essenceCost[1], tower->upgrades[0]);
			towerUpgrades[1]->SetText(towerUpgrades[1]->GetText() + "(W)");
		}
		if (tower->upgrades[1] != "")
		{
			SetUpgradeButtons(towerUpgrades[2], upgradeCosts[2], essenceCost[2], tower->upgrades[1]);
			towerUpgrades[2]->SetText(towerUpgrades[2]->GetText() + "(E)");
		}
	}
}

void CursorControl::SetUpgradeButtons(GUI* button,GUI* cost,GUI* ecost, string tower)
{
	if (tower == string("Ice"))
	{
		button->SetText("Ice Tower");
		button->b_isActive = true;
		button->meshID = GEO_ICETOWER;
		std::ostringstream os;
		os << "  " << IceTower::cost;
		cost->meshID = GEO_COIN;
		cost->scale.Set(1.f, 1.f, 1.f);
		cost->meshOffset.Set(1, 1, 0);
		cost->SetText(os.str());
		cost->b_isActive = true;
		os.str("");
		os << "  " << IceTower::ecost;
		ecost->meshID = GEO_ICEESSENCE;
		ecost->scale.Set(1.5f, 1.5f, 1.5f);
		ecost->meshOffset.Set(1, 1, 0);
		ecost->SetText(os.str());
		CostColor(ecost, IceTower::type);
		ecost->b_isActive = true;

		button->functionID = 6;
	}
	else if (tower == string("Poison"))
	{
		button->SetText("Poison Tower");
		button->b_isActive = true;
		button->meshID = GEO_POISONTOWER;
		std::ostringstream os;
		os << "  " << PoisonTower::cost;
		cost->meshID = GEO_COIN;
		cost->scale.Set(1.f, 1.f, 1.f);
		cost->meshOffset.Set(1, 1, 0);
		cost->SetText(os.str());
		cost->b_isActive = true;

		os.str("");
		os << "  " << PoisonTower::ecost;
		ecost->meshID = GEO_POISONESSENCE;
		ecost->scale.Set(1.5f, 1.5f, 1.5f);
		ecost->meshOffset.Set(1, 1, 0);
		ecost->SetText(os.str());
		CostColor(ecost, PoisonTower::type);
		ecost->b_isActive = true;

		button->functionID = 7;
	}
	else if (tower == string("Mortar"))
	{
		button->SetText("Mortar Tower");
		button->b_isActive = true;
		button->meshID = GEO_MORTARCANNON;
		std::ostringstream os;
		os << "  " << MortarTower::cost;
		cost->meshID = GEO_COIN;
		cost->scale.Set(1.f, 1.f, 1.f);
		cost->meshOffset.Set(1, 1, 0);
		cost->SetText(os.str());
		cost->b_isActive = true;

		os.str("");
		os << "  " << MortarTower::ecost;
		ecost->meshID = GEO_TANKESSENCE;
		ecost->scale.Set(1.5f, 1.5f, 1.5f);
		ecost->meshOffset.Set(1, 1, 0);		ecost->SetText(os.str());
		CostColor(ecost, MortarTower::type);
		ecost->b_isActive = true;

		button->functionID = 8;
	}
	else if (tower == string("Speed"))
	{
		button->SetText("Speed Tower");
		button->b_isActive = true;
		button->meshID = GEO_SPEEDMODEL;
		std::ostringstream os;
		os << "  " << SpeedTower::cost;
		cost->meshID = GEO_COIN;
		cost->scale.Set(1.f, 1.f, 1.f);
		cost->meshOffset.Set(1, 1, 0);
		cost->SetText(os.str());
		cost->b_isActive = true;

		os.str("");
		os << "  " << SpeedTower::ecost;
		ecost->meshID = GEO_SPEEDESSENCE;
		ecost->scale.Set(1.5f, 1.5f, 1.5f);
		ecost->meshOffset.Set(1, 1, 0);
		ecost->SetText(os.str());
		CostColor(ecost, SpeedTower::type);
		ecost->b_isActive = true;

		button->functionID = 9;
	}
}

void CursorControl::HandleButton(TileMap &tileMap,GUI* button)
{
	if (button->functionID == 0)
	{
		if (Scene::player.i_currency >= ArrowTower::cost)
		{
			SpawnTower(string("Arrow"));
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			Scene::player.i_currency -= ArrowTower::cost;
		}
	}
	else if (button->functionID == 1)
	{
		if (Scene::player.i_currency >= CannonTower::cost)
		{
			SpawnTower(string("Cannon"));
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			Scene::player.i_currency -= CannonTower::cost;
		}
	}
	else if (button->functionID == 2)
	{
		if (Scene::player.i_currency >= CaptureTower::cost)
		{
			SpawnTower(string("Capture"));
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			Scene::player.i_currency -= CaptureTower::cost;
		}
	}
	else if (button->functionID == 3)
	{
		if (Scene::player.i_currency >= BuffTower::cost)
		{
			SpawnTower(string("Buff"));
			tileMap.screenMap[checkPositionX][checkPositionY] = -3;
			Scene::player.i_currency -= BuffTower::cost;
		}
	}
	else if (button->functionID == 5) // Level up
	{
		Tower* temp = FindTower(checkPositionX, checkPositionY);
		if (Scene::player.i_currency >= temp->GetCost())
		{
			if (CheckPlayerEssence(temp->essence, temp->essenceUpgradeCost))
			{
				if (temp->LevelUp())
				{
					RemovePlayerEssence(temp->essence, temp->essenceUpgradeCost);
					Scene::player.i_currency -= temp->GetCost();
					temp->SetCost(temp->GetCost() * 2);
				}
			}
		}
	}
	else if (button->functionID == 6)
	{
		Tower* temp = FindTower(checkPositionX, checkPositionY);
		if (temp != nullptr)
		{
			if (Scene::player.i_currency >= IceTower::cost)
			{
				if (CheckPlayerEssence(IceTower::type, IceTower::ecost))
				{
					RemovePlayerEssence(IceTower::type, IceTower::ecost);
					RemoveTower(temp);
					SpawnTower("Ice");
					Scene::player.i_currency -= IceTower::cost;
				}
			}
		}
	}
	else if (button->functionID == 7)
	{
		Tower* temp = FindTower(checkPositionX, checkPositionY);
		if (temp != nullptr)
		{
			if (Scene::player.i_currency >= PoisonTower::cost)
			{
				if (CheckPlayerEssence(PoisonTower::type, PoisonTower::ecost))
				{
					RemovePlayerEssence(PoisonTower::type, PoisonTower::ecost);
					RemoveTower(temp);
					SpawnTower("Poison");
					Scene::player.i_currency -= PoisonTower::cost;
				}
			}
		}
	}
	else if (button->functionID == 8)
	{
		Tower* temp = FindTower(checkPositionX, checkPositionY);
		if (temp != nullptr)
		{
			if (Scene::player.i_currency >= MortarTower::cost)
			{
				if (CheckPlayerEssence(MortarTower::type, MortarTower::ecost))
				{
					RemovePlayerEssence(MortarTower::type, MortarTower::ecost);
					RemoveTower(temp);
					SpawnTower("Mortar");
					Scene::player.i_currency -= MortarTower::cost;
				}
			}
		}
	}
	else if (button->functionID == 9)
	{
		Tower* temp = FindTower(checkPositionX, checkPositionY);
		if (temp != nullptr)
		{
			if (Scene::player.i_currency >= SpeedTower::cost)
			{
				if (CheckPlayerEssence(SpeedTower::type, SpeedTower::ecost))
				{
					RemovePlayerEssence(SpeedTower::type, SpeedTower::ecost);
					RemoveTower(temp);
					SpawnTower("Speed");
					Scene::player.i_currency -= SpeedTower::cost;
				}
			}
		}
	}
	else if (button->functionID == 10)
	{
		tileMap.waves.StartWave();
		//std::cout << "WAVE STARTED" << std::endl;
	}
}

void CursorControl::CostColor(GUI* cost,Tower::ESSENCE_TYPE type)
{
	if (type == Tower::E_BASIC)
	{
		cost->textColor.Set(0, 1.f, 0);
		cost->meshID = GEO_POISONESSENCE;
	}
	else if (type == Tower::E_ICE)
	{
		cost->textColor.Set(0.5f, 1.f, 1);
		cost->meshID = GEO_ICEESSENCE;
	}
	else if (type == Tower::E_SPEED)
	{
		cost->textColor.Set(1.f, 1.f, 0);
		cost->meshID = GEO_SPEEDESSENCE;
	}
	else if (type == Tower::E_HEAVY)
	{
		cost->textColor.Set(1.f, 0.3f, 0.3f);
		cost->meshID = GEO_TANKESSENCE;
	}
}

bool CursorControl::CheckPlayerEssence(Tower::ESSENCE_TYPE type, int amount)
{
	if (type == Tower::E_BASIC)
	{
		if (Scene::player.i_essenceBasic >= amount)
		{
			return true;
		}
	}
	else if (type == Tower::E_ICE)
	{
		if (Scene::player.i_essenceIce >= amount)
		{
			return true;
		}
	}
	else if (type == Tower::E_SPEED)
	{
		if (Scene::player.i_essenceSpeed >= amount)
		{
			return true;
		}
	}
	else if (type == Tower::E_HEAVY)
	{
		if (Scene::player.i_essenceTanky >= amount)
		{
			return true;
		}
	}
	return false;
}

void CursorControl::RemovePlayerEssence(Tower::ESSENCE_TYPE type, int amount)
{
	if (type == Tower::E_BASIC)
	{
		Scene::player.i_essenceBasic -= amount;
	}
	else if (type == Tower::E_ICE)
	{
		Scene::player.i_essenceIce -= amount;
	}
	else if (type == Tower::E_SPEED)
	{
		Scene::player.i_essenceSpeed -= amount;
	}
	else if (type == Tower::E_HEAVY)
	{
		Scene::player.i_essenceTanky -= amount;
	}
}

void CursorControl::Clear()
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

	for (int i = 0; i < 3; ++i)
	{
		if (towerUpgrades[i] != nullptr)
		{
			delete towerUpgrades[i];
		}
		if (upgradeCosts[i] != nullptr)
		{
			delete upgradeCosts[i];
		}
		if (essenceCost[i] != nullptr)
		{
			delete essenceCost[i];
		}
	}

	for (int i = 0; i < (int)T_TOTAL; ++i)
	{
		if (towerStats[i] != nullptr)
		{
			delete towerStats[i];
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
	if (background2 != nullptr)
	{
		delete background2;
	}
	if (background3 != nullptr)
	{
		delete background3;
	}
	if (selling != nullptr)
	{
		delete selling;
	}
	if (skip != nullptr)
	{
		delete skip;
	}
}