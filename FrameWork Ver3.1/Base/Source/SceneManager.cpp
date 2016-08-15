#include "SceneManager.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "Assignment.h"

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
	this->m_currentSceneID = 1;
	EntityManager::GetInstance()->m_currentSceneID = this->m_currentSceneID;
	RenderManager::GetInstance()->Init();
	//create scenes here
	CreateScene(new Assignment());
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
		EntityManager::GetInstance()->m_currentSceneID = scene->m_sceneID;
		sceneList.push_back(scene);
		scene->Init();

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
}