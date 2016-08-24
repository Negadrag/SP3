#ifndef EDITORCURSOR_H
#define EDITORCURSOR_H

#include "Cursor.h"
#include "GUI.h"

class EditorCursor : public Cursor
{
public:
	enum TILE_TYPE
	{
		TILE_OPEN,
		TILE_EMPTY,
		TILE_NODE,
		TILE_PATH,
		TILE_END
	};

	EditorCursor();
	~EditorCursor();

	void Init(TileMap *tileMap);
	void Update(OrthoCamera &camera, const double &dt);
	int checkPositionX, checkPositionY;

	TileMap *tileMap;
	TILE_TYPE currentTile;

	Node* root;
	GUI* save;
private:
	bool bLButtonState;
	void Clicking();
	int tile[TILE_END];
	void HotKeys();
	void AddNode(Node *node, int x, int y);
	Node* FindNode(int number);
	void RemoveFromMap(Node* node);
};

#endif