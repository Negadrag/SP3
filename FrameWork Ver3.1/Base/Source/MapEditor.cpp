#include "MapEditor.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"

#include <windows.h>

MapEditor::MapEditor() :Scene()
{
	f_runTimer = 0.f;
	b_startScene = false;
}

MapEditor::~MapEditor()
{
}

void MapEditor::Init()
{
	if (b_startScene == true)
	{
		HWND hwnd = GetConsoleWindow();
		SetForegroundWindow(hwnd);
		HandleInput();
		tileMap.LoadEmpty();
		//this->m_sceneID = 1;

		tileMap.waves.player = &(this->player);


		camera.Init(Vector3((float)(tileMap.i_columns - 1) / 2.f, (float)tileMap.i_rows / 2.f, 10.f), Vector3((float)(tileMap.i_columns - 1) / 2.f, (float)tileMap.i_rows / 2.f, 0.f), Vector3(0, 1, 0), 30.f);

		//camera.Init(Vector3(0,-5,10), Vector3(0,0,0), Vector3(0, 1, 0));
		camera.b_ortho = true;
		camera.orthoSize = (tileMap.i_rows / 2) + 1;
		camera.defaultOrtho = camera.orthoSize;
		camera.aspectRatio.Set(4, 3);
		RenderManager::GetInstance()->SetCamera(&camera);

		grass.meshID = GEO_GRASS_DARKGREEN;
		grass.pos.Set((float)(tileMap.i_columns - 1) / 2.f, (float)tileMap.i_rows / 2.f, 0);
		grass.scale.Set(camera.orthoSize * (camera.aspectRatio.x / camera.aspectRatio.y) * 2, camera.orthoSize * 2.5, 1);
		grass.rotation.Set(0, 0, 0);

		cursor.Init(&tileMap);
	}
}

void MapEditor::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
	if (f_runTimer < 1.f)
	{
		f_runTimer += (float)dt;
	}

	if (f_runTimer >= 1.f && b_startScene == false)
	{
		b_startScene = true;
		Init();
	}

	if (b_startScene == true)
	{
		fps = (float)(1.f / dt);

		cursor.Update(camera, dt);
		camera.Update(dt);
		RenderManager::GetInstance()->SetCamera(&camera);
	}
}

void MapEditor::Render()
{
	if (b_startScene)
	{
		RenderManager::GetInstance()->RenderMesh(GEO_CONE, Vector3(cursor.checkPositionX, cursor.checkPositionY, 0), Vector3(1.5f, 1.5f, 1.5f), Vector3(90, 0, 0), false, false);

		for (int i = 0; i < tileMap.i_rows; ++i) // y - axis
		{
			for (int j = 0; j < tileMap.i_columns; ++j) // x - axis
			{
				if (tileMap.screenMap[j][i] == -2)
				{
					RenderManager::GetInstance()->RenderMesh(GEO_CUBE2, Vector3(j * tileMap.i_tileSize, i  * tileMap.i_tileSize, 0), Vector3(1, 1, 1), Vector3(0, -90, 0), true, false);
				}
				else if (tileMap.screenMap[j][i] == -1)
				{
					RenderManager::GetInstance()->RenderMesh(GEO_PATH, Vector3(j * tileMap.i_tileSize, i  * tileMap.i_tileSize, 0.1), Vector3(1, 1, 1), Vector3(0, 0, 0), true, false);
				}
				else if (tileMap.screenMap[j][i] == 0)
				{
					RenderManager::GetInstance()->RenderMesh(GEO_GRASS, Vector3(j * tileMap.i_tileSize, i  * tileMap.i_tileSize, 0.1), Vector3(1, 1, 1), Vector3(0, 0, 0), true, false);
				}
				else if (tileMap.screenMap[j][i] > 0)
				{
					//RenderManager::GetInstance()->RenderMesh(GEO_SPHERE, Vector3(j * tileMap.i_tileSize, i  * tileMap.i_tileSize, 0.1), Vector3(1, 1, 1), Vector3(0, 0, 0), true, false);
					std::ostringstream ss;
					ss << tileMap.screenMap[j][i];
					RenderManager::GetInstance()->RenderText(ss.str(), Color(1, 1, 0), Vector3((float)(j * tileMap.i_tileSize) - (float)tileMap.i_tileSize * 0.5f, (float)(i  * tileMap.i_tileSize) - (float)tileMap.i_tileSize * 0.5f, 0.1), Vector3(1, 1, 1), Vector3(0, 0, 0));
				}
			}
		}

		RenderManager::GetInstance()->RenderTextOnScreen("Q - Node, W - Tower Slot, E - Remove", Color(0, 1, 0), 3, 15, 57);

		if (cursor.currentTile == EditorCursor::TILE_EMPTY)
		{
			RenderManager::GetInstance()->RenderTextOnScreen("REMOVE", Color(0, 1, 0), 3, 33, 53);
		}
		else if (cursor.currentTile == EditorCursor::TILE_NODE)
		{
			RenderManager::GetInstance()->RenderTextOnScreen("NODE", Color(0, 1, 0), 3, 33, 53);
		}
		else if (cursor.currentTile == EditorCursor::TILE_OPEN)
		{
			RenderManager::GetInstance()->RenderTextOnScreen("TOWER SLOTS", Color(0, 1, 0), 3, 33, 53);
		}

		//On screen text
		std::ostringstream ss;
		ss.precision(5);
		ss << "FPS: " << fps;
		RenderManager::GetInstance()->RenderTextOnScreen(ss.str(), Color(0, 1, 0), 3, 0, 9);
	}
	else
	{
		RenderManager::GetInstance()->RenderTextOnScreen("LOOK AT CONSOLE", Color(1, 1, 0), 3, 20, 30);
	}

}

void MapEditor::HandleInput()
{
	{
		string input1, input2;

		std::cout << "MAP EDITOR" << std::endl;
		std::cout << "Map Name:";
		std::cin >> s_mapName;
		std::cout << "Number of columns (x - axis) :";
		std::cin >> input1;
		tileMap.i_columns = atoi(input1.c_str());
		std::cout << "Number of rows (y - axis) :";
		std::cin >> input2;
		tileMap.i_rows = atoi(input2.c_str());
	}

	int numWaves;
	{
		string input;
		std::cout << "Number of waves:";
		std::cin >> input;
		numWaves = atoi(input.c_str());
	}

	for (int i = 0; i < numWaves; ++i)
	{
		std::cout << "Wave " << i << " enemies:" << std::endl;

		std::ostringstream ss;
		// enemies
		for (int j = 0; j < 5; ++j)
		{
			int enemy;
			string input;
			std::cout << "Enemy " << j << " (0 = STOP,1 = MINION, 2 = ICE, 3 = SPEED, 4 = TANK):";
			std::cin >> input;
			enemy = atoi(input.c_str());
			switch (enemy)
			{
			case (0) :
				break;
			case (1) :
				ss << "MINION,";
				break;
			case (2) :
				ss << "ICE,";
				break;
			case (3) :
				ss << "SPEED,";
				break;
			case (4) :
				ss << "TANK,";
				break;
			default:
				ss << "MINION,";
				break;
			}
			if (enemy == 0)
			{
				break;
			}
		}

		ss << "/,";
		// revolutions
		{
			string input;
			std::cout << "Number of revolutions:";
			std::cin >> input;
			ss << atoi(input.c_str()) << ',';
		}

		// frequency
		{
			string input;
			std::cout << "Frequency:";
			std::cin >> input;
			ss << atoi(input.c_str()) << ',';
			if (i != numWaves - 1)
			{
				ss << '\n';
			}
		}
		waves.push_back(ss.str());
	}
}
void MapEditor::WriteToFile()
{
	std::ofstream file;
	file.open("Maps//" + s_mapName + ".csv");
	for (int i = tileMap.i_rows - 1; i >= 0; --i)
	{
		for (int j = 0; j < tileMap.i_columns; ++j)
		{
			int num = tileMap.screenMap[j][i];
			if (num != 0)
				file << num;
			if (j != tileMap.i_columns - 1)
				file << ',';
		}
		file << '\n';
	}
	file << "e,\n";
	file << "/,0,0,\n";
	for (vector<string>::iterator it = waves.begin(); it != waves.end(); ++it)
	{
		file << *it;
	}

	file.close();
}

void MapEditor::Exit()
{
	WriteToFile();
	//clean Up scene Variables
}