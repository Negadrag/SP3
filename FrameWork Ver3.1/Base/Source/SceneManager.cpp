#include "SceneManager.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "Assignment.h"
#include "TestScene.h"
#include "CaptureGame.h"
#include "Application.h"

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

	RenderManager::GetInstance()->Init();
	//create scenes here
	//EntityManager::GetInstance()->m_currentSceneID = 1;
	CreateScene(new Assignment());
	//EntityManager::GetInstance()->m_currentSceneID = 2;
	CreateScene(new TestScene());
	//EntityManager::GetInstance()->m_currentSceneID = 3;
	CreateScene(new CaptureGame());

	this->m_currentSceneID = 1;
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
		}
	}
	EntityManager::GetInstance()->m_currentSceneID = this->m_currentSceneID;
	EntityManager::GetInstance()->UpdateAllEntity(dt, m_currentSceneID);
	RenderManager::GetInstance()->Update(dt);
	RenderManager::GetInstance()->UpdateBillboard(m_currentSceneID);
	GUIManager::GetInstance()->m_currentSceneID = m_currentSceneID;

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
	//RenderManager::GetInstance()->ClearShadows();
	GUIManager::GetInstance()->RenderAllGUI();
	
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
	if (SceneExist(sceneID) == true)
	{
		for (list<Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
		{
			if ((*it)->m_sceneID == sceneID)
			{
				(*it)->Exit();
				(*it)->Init();
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
}

bool SceneManager::ChangeScene(int sceneID,bool freezeScene)
{
	if (sceneID == m_currentSceneID)
	{
		return false;
	}
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