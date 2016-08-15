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

	void Init();
	void Update(double dt);
	void Render();

	void CreateScene(Scene* scene);
	bool SceneExist(int sceneID);
	void ReinstanceScene(int sceneID);
	void ChangeScene(int sceneID);
	void Exit();
	static SceneManager* GetInstance();

private:
	SceneManager();
	static SceneManager* instance;
protected:

};

#endif