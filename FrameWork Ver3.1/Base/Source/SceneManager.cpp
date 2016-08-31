#include "SceneManager.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "Music.h"

#include "Assignment.h"
#include "TestScene.h"
#include "CaptureGame.h"
#include "Application.h"
#include "MapEditor.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "Display.h"
#include "MainMenu.h"
#include "LevelTwo.h"
#include "Pause.h"
#include "CustomLevel.h"
#include "LevelFour.h"
#include "LevelThree.h"


SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{
	m_currentSceneID = 0;
}

SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
	sceneToExit = nullptr;
	RenderManager::GetInstance()->Init();
	Music::GetInstance()->Init();
	//create scenes here
	//EntityManager::GetInstance()->m_currentSceneID = 1;
	CreateScene(new Assignment());
	//EntityManager::GetInstance()->m_currentSceneID = 2;
	CreateScene(new TestScene());
	//EntityManager::GetInstance()->m_currentSceneID = 3;
	CreateScene(new CaptureGame());
	// Scene ID 4
	CreateScene(new MapEditor());
	// Scene ID 5
	CreateScene(new WinScene());
	// Scene ID 6
	CreateScene(new LoseScene());
	// Scene ID 7
	CreateScene(new Display());
	// Scene ID 8
	CreateScene(new MainMenu());
	// Scene ID 9
	CreateScene(new LevelTwo());
	// Scene ID 10
	CreateScene(new Pause());
	// Scene ID 11
	CreateScene(new CustomLevel());
	// Scene ID 12
	CreateScene(new LevelFour());
	// Scene ID 13
	CreateScene(new LevelThree());

	this->m_currentSceneID = 8;
	EntityManager::GetInstance()->m_currentSceneID = this->m_currentSceneID;
	GUIManager::GetInstance()->m_currentSceneID = this->m_currentSceneID;
	for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
	{
		if ((*it)->m_sceneID == m_currentSceneID)
		{
			(*it)->Init();
		}
	}
}

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager();
	}
	return instance;
}

void SceneManager::Update(double dt)
{
	if (dt > 0.1)
	{
		dt = 0.1;
	}
	if (Application::IsKeyPressed('B'))
	{
		dt *= 2.f;
	}
	Scene* currScene;
	for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
	{
		if ((*it)->m_sceneID == m_currentSceneID)
		{
			currScene = (*it);
		}
	}
	if (currScene->b_changeScene == true)
	{
		if (currScene->b_frozen == false)
		{
			currScene->Exit();
		}
		currScene->b_changeScene = false;
		m_currentSceneID = m_nxtSceneID;

		GUIManager::GetInstance()->m_currentSceneID = m_currentSceneID;

		Scene* nxtScene;
		for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
		{
			if ((*it)->m_sceneID == m_currentSceneID)
			{
				nxtScene = (*it);
			}
		}
		if (nxtScene->b_frozen == false)
		{
			EntityManager::GetInstance()->m_currentSceneID = nxtScene->m_sceneID;
			nxtScene->Init();
		}
		else
		{
			nxtScene->b_frozen = false;
			nxtScene->Init2();
		}
	}

	if (sceneToExit != nullptr)
	{
		sceneToExit->Exit();
		sceneToExit = nullptr;
	}

	EntityManager::GetInstance()->m_currentSceneID = this->m_currentSceneID;
	EntityManager::GetInstance()->UpdateAllEntity(dt, m_currentSceneID);
	RenderManager::GetInstance()->Update(dt);
	RenderManager::GetInstance()->UpdateBillboard(m_currentSceneID);
	

	for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
	{
		if ((*it)->m_sceneID == m_currentSceneID)
		{
			(*it)->Update(dt);
		}
	}

}

void SceneManager::Render()
{
	RenderManager::GetInstance()->RenderGPass(m_currentSceneID);

	RenderManager::GetInstance()->RenderMain(m_currentSceneID);
	
	for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
	{
		if ((*it)->m_sceneID == m_currentSceneID)
		{
			(*it)->Render();
		}
	}
	GUIManager::GetInstance()->RenderAllGUI(m_currentSceneID);
	//RenderManager::GetInstance()->ClearShadows();
	
	
}

bool SceneManager::SceneExist(int sceneID)
{
	for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
	{
		if ((*it)->m_sceneID == sceneID)
		{
			return true;
		}
	}
	return false;
}

void SceneManager::CreateScene(Scene* scene)
{
	if (SceneExist(scene->m_sceneID) == false)
	{
		//EntityManager::GetInstance()->m_currentSceneID = scene->m_sceneID;
		sceneList.push_back(scene);
		//scene->Init();
	}
	else
	{
		delete scene;
	}
}

void SceneManager::ReinstanceScene(int sceneID)
{
	ChangeScene(sceneID, false);
}

void SceneManager::ExitScene(int sceneID)
{
	if (SceneExist(sceneID) == true)
	{
		for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
		{
			if ((*it)->m_sceneID == sceneID)
			{
				sceneToExit = *it;
				(*it)->b_frozen = false;
			}
		}
	}
}

void SceneManager::Exit()
{
	for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
	{
		delete (*it);
	}
	delete EntityManager::GetInstance();
	delete RenderManager::GetInstance();
	delete GUIManager::GetInstance();
	delete Music::GetInstance();
}

bool SceneManager::ChangeScene(int sceneID,bool freezeScene)
{
	Music::GetInstance()->StopAllSound(); 
	// stop the music that are playing in the current scene

	if (SceneExist(sceneID) == true)
	{
		//Scene* nxtScene;
		Scene* currScene;
		for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
		{
			/*if ((*it)->m_sceneID == sceneID)
			{
				nxtScene = (*it);
			}*/
			if ((*it)->m_sceneID == m_currentSceneID)
			{
				currScene = (*it);
			}
		}
		currScene->b_frozen = freezeScene;
		currScene->b_changeScene = true;
		m_nxtSceneID = sceneID;
		return true;
	}
	else
	{
		return false;
	}
}