#include "EditorCursor.h"
#include "Application.h"
#include "MathUtility.h"


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
}

void EditorCursor::Init(TileMap *tileMap)
{
	this->tileMap = tileMap;

	tile[TILE_OPEN] = -2;
	tile[TILE_EMPTY] = 0;
	tile[TILE_NODE] = 1;
	tile[TILE_PATH] = -1;
}

void EditorCursor::Update(OrthoCamera &camera, const double &dt)
{
	Cursor::Update(camera, *tileMap, dt);

	checkPositionX = (int)Math::Clamp(worldCoords.x, 0.f, (float)tileMap->i_columns - 1.f);
	checkPositionY = (int)Math::Clamp(worldCoords.y, 0.f, (float)tileMap->i_rows - 1.f);

	camera.orthoSize = Math::Clamp(camera.orthoSize - (float)Application::mouse_scroll, 2.f, camera.defaultOrtho); // scrolling in and out
	EdgePanning(dt, camera, worldX, worldY, 6 * tileMap->i_rows);

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
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		if (tileMap != nullptr)
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
			else if (tileMap->screenMap[checkPositionX][checkPositionY] > 0)
			{
				Node *temp;
				temp = FindNode(tileMap->screenMap[checkPositionX][checkPositionY]);
				RemoveFromMap(temp);
				tile[TILE_NODE] = temp->i_number;
				delete temp;
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