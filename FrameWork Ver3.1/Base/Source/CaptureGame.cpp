#include "CaptureGame.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"

CaptureGame::CaptureGame(int sceneId) :Scene(sceneId)
{
}

CaptureGame::~CaptureGame()
{
}

void CaptureGame::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	m_speed = 1.f;
	m_gravity.Set(0, -10, 0);
	Math::InitRNG();

	m_objectCount = 0;
	
	m_ghost = new GameObject(GameObject::GO_BALL);
	

	CreateScene();
	camera.Init(Vector3(0, 0, 1500), Vector3(0, 0, 0), Vector3(0, 1, 0));
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;

	std::cout << "entered";

}

void CaptureGame::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (b_initScene == false)
	{
		
		b_initScene = true;
	}
	

	fps = (float)(1.f / dt);

	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);





	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "down" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		x = m_worldWidth * (x / w);
		y = m_worldHeight * ((h - y) / h);

		m_ghost->pos.Set(x, y, 0);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "up" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		x = m_worldWidth * (x / w);
		y = m_worldHeight * ((h - y) / h);

		GameObject * ball = FetchGO(GameObject::GO_BALL);
		//ball->type = GameObject::GO_BALL;
		//ball->pos.Set(0, 0, 0);// = m_ghost->pos;
		ball->pos.Set(0, 300, 0);
		ball->vel.Set(0, 0, 0);// *= 0.98f;// = m_ghost->pos - Vector3(x, y, 0);
		ball->scale.Set(24, 24, 24);
		ball->mass = 1.f;
		m_ghost->b_isActive = false;

		GameObject * ball2 = FetchGO(GameObject::GO_BALL);
		ball2->pos.Set(-40, 300, 0);
		ball2->vel.Set(0, 0, 0);
		ball2->scale.Set(24, 24, 24);
		ball2->mass = 1.f;

		EstimatedTime = -1;
		TimeTaken = 0;
		timerStarted = true;

	}


	/*if (dt >= temp)
	{
		GameObject * ball = FetchGO(GameObject::GO_BALL);
		ball->vel.Set(0, 0, 0);
		ball->scale.Set(1.5, 1.5, 1.5);
		ball->mass = 1.f;
	}*/

	std::cout << m_objectCount << std::endl;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end();)
	{
		
		GameObject *go = (GameObject *)*it;
		if (go->b_isActive)
		{
			if (go->type == GameObject::GO_BALL)
			{
				go->vel += m_gravity;
				go->pos += go->vel * dt * m_speed;
				//go->vel *= 0.98;

				float radius = go->scale.x;

				/*if ((go->pos.x > m_worldWidth - radius && go->vel.x > 0) ||
					(go->pos.x < radius && go->vel.x < 0))
					go->vel.x *= -1;*/

				if ((go->pos.y > 400) ||
					(go->pos.y < -400))
				{
					go->b_isActive = false;//vel.y *= -1;
					delete go;
					it = m_goList.erase(it);
					m_objectCount--;
					continue;
				}

				if (go->pos.z >= 5)
				{
					go->b_isActive = false;
					delete go;
					it = m_goList.erase(it);
					m_objectCount--;
					continue;
				}
				if (go->pos.z <= -5)
				{
					go->b_isActive = false;
					delete go;
					it = m_goList.erase(it);
					m_objectCount--;
					continue;
				}
			}

			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject*other = (GameObject*)*it2;
				if (!other->b_isActive) continue;

				GameObject *goA = go, *goB = other;
				if (go->type != GameObject::GO_BALL)
				{
					if (other->type != GameObject::GO_BALL)
						continue;
					goA = other;
					goB = go;
				}
				if (CheckCollision(goA, goB, dt))
				{
					timerStarted = false;
					CollisionResponse(goA, goB);
					break;
				}
			}
			++it;
		}
	}
}

void CaptureGame::Render()
{
	
	//if (m_ghost->b_isActive)
		//RenderGO(m_ghost);

	/*for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->b_isActive)
		{
			RenderGO(go);
		}
	}*/

}

GameObject* CaptureGame::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	/*for (std::vector<GameObject*>::iterator iter = m_goList.begin(); iter != m_goList.end(); ++iter)
	{
		GameObject *go = *iter;
		if (go->b_isActive == false)
		{
			go->b_isActive = true;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned int i = 0; i < 100; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	GameObject *go = *(m_goList.end() - 1);
	go->b_isActive = true;
	++m_objectCount;*/
	GameObject* go;
	GameObject* go2;
	if (type == GameObject::GO_BALL)
	{
		go = new GameObject();
		go->type = GameObject::GO_BALL;
		go->meshID = GEO_SPHERE;
		go->pos.Set(0, 0, 0);
		go->scale.Set(1, 1, 1);
		go->vel.Set(0, 0, 0);
		go->b_isActive = true;
		m_goList.push_back(go);
		m_objectCount++;
		
	}
	if (type == GameObject::GO_WALL)
	{
		go = new GameObject();
		go->type = GameObject::GO_WALL;
		go->meshID = GEO_CUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}


	return go;
}

bool CaptureGame::CheckCollision(GameObject* go, GameObject* other, double dt)
{
	switch (other->type)
	{
	case GameObject::GO_BALL:
	{
		float distanceSquared = ((go->pos + go->vel*dt) - (other->pos - other->vel * dt)).LengthSquared();
		float combineRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);

		Vector3 relativeVelocity = go->vel - other->vel;
		Vector3 relativeDisplacement = other->pos - go->pos;

		return distanceSquared < combineRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0;
		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos ;
		Vector3 N = other->normal;

		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
		{
			N = -N;
		}
		float r = go->scale.x/2;
		float h = other->scale.x/2;
		float l = other->scale.y/2;
		Vector3 NP = Vector3(-N.y, N.x);

		return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go->pos;
		Vector3 p2 = other->pos;
		float r1 = go->scale.x;
		float r2 = other->scale.x;
		Vector3 u = go->vel;
		float combinedR = r1 + r2;


		return ((p2 - p1).LengthSquared() < combinedR * combinedR && (p2 - p1).Dot(u) > 0);
	}
	}
}

float CaptureGame::CheckCollision2(GameObject* go, GameObject* other)
{
	switch (other->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 rel = go->vel - other->vel;
		Vector3 dir = go->pos - other->pos;
		float r = go->scale.x + other->scale.x;
		float dot = rel.Dot(dir);
		if (dot > 0) return -1;
		float a = rel.LengthSquared();//rel.dot(rel)
		float b = 2 * rel.Dot(dir);
		float c = dir.LengthSquared() - r * r;//dir.dot(dir) - r * r
		float d = b*b - 4 * a*c;
		if (d < 0) return-1;
		float t = (-b - sqrt(d)) / (2 * a);
		if (t < 0) t = (-b + sqrt(d)) / (2 * a);
		return t;
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos;
		Vector3 N = other->normal;
		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
			N = -N;
		Vector3 U = go->vel;
		if (U.Dot(N) < 0)
			return -1;
		float r = go->scale.x;
		float h = other->scale.x;
		w0 -= (r + h * 0.5f) * N;

		float dist = (w0 - b1).Dot(N);
		float speed = U.Dot(N);

		float th = dist / speed;

		Vector3 NP = Vector3(-N.y, N.x);

		float l = other->scale.y;
		Vector3 w1 = w0 + (l * 0.5f) * NP;
		Vector3 w2 = w0 - (l * 0.5f) * NP;

		Vector3 bh = b1 + U * th;

		if ((w1 - bh).Dot(w2 - bh) > 0)
			return -1;

		return th;
	}
	
	}
}

void CaptureGame::CollisionResponse(GameObject* go, GameObject* other)
{
	Vector3 u1n, u2n;
	Vector3 N, u, uN;
	switch (other->type)
	{
	case GameObject::GO_BALL:
		m1 = go->mass;
		m2 = other->mass;
		u1 = go->vel;
		u2 = other->vel;

		N = (other->pos - go->pos).Normalized();
		u1n = u1.Dot(N) * N;
		u2n = u2.Dot(N) * N;

		go->vel = u1 + 2 * m2 / (m1 + m2) * (u2n - u1n);
		other->vel = u2 + 2 * m1 / (m1 + m2) * (u1n - u2n);

		v1 = go->vel;
		v2 = other->vel;
		break;
	
	case GameObject::GO_WALL:
		u = go->vel * 0.98;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		break;

	case GameObject::GO_PILLAR:
		u1 = go->vel;
		Vector3 N = (other->pos - go->pos).Normalized();
		Vector3 u1n = u1.Dot(N) * N;
		go->vel = u - 2 * u1n;

		break;
	}
}

//void CaptureGame::RenderGO(GameObject *go)
//{
//	float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
//
//	switch (go->type)
//	{
//	case GameObject::GO_BALL:
//		ren_ball.meshID = GEO_SPHERE;
//		ren_ball.pos.Set(go->pos.x, go->pos.y, go->pos.z);
//		ren_ball.scale.Set(go->scale.x, go->scale.y, go->scale.z);
//		break;
//
//	case GameObject::GO_WALL:
//		ren_wall.meshID = GEO_CUBE;
//		ren_wall.pos.Set(go->pos.x, go->pos.y, go->pos.z);
//		ren_wall.rotation.Set(0, 0, angle);
//		ren_wall.scale.Set(go->scale.x, go->scale.y, go->scale.z);
//		break;
//
//	case GameObject::GO_PILLAR:
//		ren_pillar.meshID = GEO_SPHERE;
//		ren_pillar.pos.Set(go->pos.x, go->pos.y, go->pos.z);
//		ren_pillar.rotation.Set(0, 0, angle);
//		ren_pillar.scale.Set(go->scale.x, go->scale.y, go->scale.z);
//		break;
//	}
//}

void CaptureGame::CreateScene()
{
	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(1000, 1000, 1000);
	grass.rotation.Set(-90, 0, 0);
	grass.rotation.Set(0, 0, 90);


	GameObject* ball = FetchGO(GameObject::GO_BALL);
	ball->pos.Set(0, 1, 0);

	GameObject* ball2 = FetchGO(GameObject::GO_BALL);
	ball2->pos.Set(-40, 1, 0);


	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			GameObject* wall = FetchGO(GameObject::GO_WALL);
			wall->pos.Set(-300 + (i * 150), 200 + (j * -300), 0);
			wall->scale.Set(30, 30, 15);
			wall->rotation.Set(0, 0, 45);
			Mtx44 rotate;
			rotate.SetToRotation(-45, 0, 0, 1);
			wall->normal = rotate* wall->normal;

			GameObject* wall2 = FetchGO(GameObject::GO_WALL);
			wall2->pos.Set(-300 + (i * 150), 200 + (j * -300), 0);
			wall2->scale.Set(30, 30, 15);
			wall2->rotation.Set(0, 0, -45);
			rotate.SetToIdentity();
			rotate.SetToRotation(45, 0, 0, 1);
			wall2->normal = rotate* wall2->normal;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			GameObject* wall = FetchGO(GameObject::GO_WALL);
			wall->pos.Set(-230 + (i * 150), 50 + (j * -300), 0);
			wall->scale.Set(30, 30, 15);
			wall->rotation.Set(0, 0, 45);
			Mtx44 rotate;
			rotate.SetToRotation(-45, 0, 0, 1);
			wall->normal = rotate* wall->normal;

			GameObject* wall2 = FetchGO(GameObject::GO_WALL);
			wall2->pos.Set(-230 + (i * 150), 50 + (j * -300), 0);
			wall2->scale.Set(30, 30, 15);
			wall2->rotation.Set(0, 0, -45);
			rotate.SetToIdentity();
			rotate.SetToRotation(45, 0, 0, 1);
			wall2->normal = rotate* wall2->normal;
		}
	}


	/*ren_ball = new GameObject();
	ren_ball->type = GameObject::GO_BALL;
	ren_ball->meshID = GEO_SPHERE;
	ren_ball->pos.Set(0, 0, 0);
	ren_ball->scale.Set(1, 1, 1);
	m_goList.push_back(ren_ball);

	GameObject * wall = FetchGO();
	ren_wall->type = GameObject::GO_WALL;
	ren_wall->pos.Set(0, -300, 0);
	ren_wall->scale.Set(105, 15, 15);
	ren_wall->b_isActive = true;*/
	
}

void CaptureGame::ClearScene()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->b_isActive)
		{
			go->b_isActive = false;
		}
	}
}


void CaptureGame::Exit()
{
	//clean Up scene Variables
	/*while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}*/

	for (vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		delete (*it);
	}
	m_goList.clear();

	/*while (particleList.size() > 0)
	{
		ParticleObject* particle = particleList.back();
		delete particle;
		particleList.pop_back();
	}*/
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
