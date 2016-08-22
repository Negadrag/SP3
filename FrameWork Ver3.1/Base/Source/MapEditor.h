#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "TileMap.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "OrthoCamera.h"
#include "EditorCursor.h"


using std::vector;

class MapEditor : public Scene
{

public:
	MapEditor();
	~MapEditor();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	void HandleInput();
	void WriteToFile();

	vector<string> waves;

	OrthoCamera camera;
	Renderable grass;

	string s_mapName;
	TileMap tileMap;
	EditorCursor cursor;

	float fps;
};

#endif