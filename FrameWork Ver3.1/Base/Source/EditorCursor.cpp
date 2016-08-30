#include "EditorCursor.h"
#include "Application.h"
#include "MathUtility.h"
#include "SceneManager.h"


EditorCursor::EditorCursor()
{
	this->tileMap = nullptr;
	checkPositionX = 0;
	checkPositionY = 0;
	currentTile = TILE_EMPTY;
	root = nullptr;
	bLButtonState = false;
}

EditorCursor::~EditorCursor()
{
	if (save != nullptr)
	{
		delete save;
	}
}

void EditorCursor::Init(TileMap *tileMap)
{
	this->tileMap = tileMap;

	tile[TILE_OPEN] = -2;
	tile[TILE_EMPTY] = 0;
	tile[TILE_NODE] = 1;
	tile[TILE_PATH] = -1;

	save = new GUI();
	save->b_buttonActive = true;
	save->b_textActive = false;
	save->meshID = GEO_SAVE;
	save->scale.Set(10, 10, 0);
	save->buttonSize.Set(10, 10);
	save->position.Set(70, 50);
	save->meshOffset.Set(5, 5, 0);
	save->b_lightEnabled = false;
	save->b_isActive = true;
	save->functionID = 0;
}

void EditorCursor::Update(OrthoCamera &camera, const double &dt)
{
	Cursor::Update(camera, *tileMap, dt);

	checkPositionX = (int)Math::Clamp(worldCoords.x, 0.f, (float)tileMap->i_columns - 1.f);
	checkPositionY = (int)Math::Clamp(worldCoords.y, 0.f, (float)tileMap->i_rows - 1.f);

	camera.orthoSize = Math::Clamp(camera.orthoSize - (float)Application::mouse_scroll, 2.f, camera.defaultOrtho); // scrolling in and out
	EdgePanning(dt, camera, screenX, screenY, 6.f * tileMap->i_rows);
	CameraBounds(camera);

	Clicking();
	HotKeys();
}

void EditorCursor::HotKeys()
{
	if (Application::IsKeyPressed('Q'))
	{
		currentTile = TILE_NODE;
	}
	else if (Application::IsKeyPressed('W'))
	{
		currentTile = TILE_OPEN;
	}
	else if (Application::IsKeyPressed('E'))
	{
		currentTile = TILE_EMPTY;
	}
}

void EditorCursor::Clicking()
{
	GUI* temp = GUIManager::GetInstance()->FindGUI(this->screenX, this->screenY);
	if (save == temp)
	{
		save->rotation.Set(10, 0, 0);
	}
	else
	{
		save->rotation.Set(0, 0, 0);
	}

	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		GUI* temp = GUIManager::GetInstance()->FindGUI(this->screenX, this->screenY);
		if (temp != nullptr)
		{
			if (temp->functionID == 0)
			{
				SceneManager::GetInstance()->ChangeScene(1, false);
			}
		}
		else if (tileMap != nullptr)
		{
			if (tileMap->screenMap[checkPositionX][checkPositionY] == tile[TILE_EMPTY])
			{
				if (currentTile == TILE_NODE)
				{
					if (tile[TILE_NODE] > 1)
					{
						if (checkPositionX == (int)FindNode(tile[currentTile] - 1)->coords.x || checkPositionY == (int)FindNode(tile[currentTile] - 1)->coords.y)
						{
							tileMap->screenMap[checkPositionX][checkPositionY] = tile[currentTile];
							AddNode(root, checkPositionX, checkPositionY);
							tile[currentTile]++;
						}
					}
					else
					{
						tileMap->screenMap[checkPositionX][checkPositionY] = tile[currentTile];
						AddNode(root, checkPositionX, checkPositionY);
						tile[currentTile]++;
					}
				}
				else
				{
					tileMap->screenMap[checkPositionX][checkPositionY] = tile[currentTile];
				}
			}
			else if (tileMap->screenMap[checkPositionX][checkPositionY] > 0 && currentTile == TILE_EMPTY)
			{
				if (tileMap->screenMap[checkPositionX][checkPositionY] == 1)
				{
					RemoveFromMap(root);
					delete root;
					root = nullptr;
					tile[TILE_NODE] = 1;
				}
				else
				{
					Node *temp;
					temp = FindNode(tileMap->screenMap[checkPositionX][checkPositionY]);
					tile[TILE_NODE] = tileMap->screenMap[checkPositionX][checkPositionY];
					Node *temp2 = FindNode(tileMap->screenMap[checkPositionX][checkPositionY] - 1);
					RemoveFromMap(temp);
					delete temp2->next;
					temp2->next = nullptr;
					
				}

			}
			else if (currentTile == TILE_EMPTY)
			{
				tileMap->screenMap[checkPositionX][checkPositionY] = tile[TILE_EMPTY];
			}
		}
		
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
}

void EditorCursor::AddNode(Node *node, int x, int y)
{
	if (root == nullptr)
	{
		root = new Node(x, y);
		root->i_number = tile[TILE_NODE];
	}
	else if (node->next == nullptr)
	{
		node->next = new Node(x, y);
		node->next->i_number = tile[TILE_NODE];
	}
	else
	{
		AddNode(node->next, x, y);
	}
}

Node* EditorCursor::FindNode(int number)
{
	Node* temp = root;
	while (temp != nullptr)
	{
		if (temp->i_number == number)
		{
			return temp;
		}
		temp = temp->next;
	}
	return temp;
}

void EditorCursor::RemoveFromMap(Node* node)
{
	tileMap->screenMap[(int)node->coords.x][(int)node->coords.y] = tile[TILE_EMPTY];
	if (node->next != nullptr)
	{
		RemoveFromMap(node->next);
	}
}