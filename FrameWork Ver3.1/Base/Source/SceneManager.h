#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <list>
#include "Scene.h"

using std::list;

class SceneManager
{
public:
	~SceneManager();

	list<Scene*> sceneList;
	int m_currentSceneID;
	int m_nxtSceneID;
	void Init();
	void Update(double dt);
	void Render();

	void CreateScene(Scene* scene);
	bool SceneExist(int sceneID);
	void ReinstanceScene(int sceneID);
	void ExitScene(int sceneID);
	bool ChangeScene(int sceneID,bool freezeScene);
	void Exit();
	static SceneManager* GetInstance();
	Scene *sceneToExit;
private:
	SceneManager();
	static SceneManager* instance;
protected:

};

#endif