#include "CaptureGame.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"

CaptureGame::CaptureGame() :Scene()
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
	resource1 = 0;
	resource2 = 0;
	resource3 = 0;
	resource4 = 0;

	balls = 20;
	bonuscount = 0;

	isrunning = false;
	b_allBallsdespawned = true;

	m_ghost = new GameObject(GameObject::GO_BALL);


	CreateScene();
	camera.Init(Vector3(0, 0, 1500), Vector3(0, 0, 0), Vector3(0, 1, 0));
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	f_ballSpawnTimer = 0.f;
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

	//if (b_initScene == false)
	//{
	//	
	//	b_initScene = true;
	//}
	if (Application::IsKeyPressed('U'))
		balls = 30;

	fps = (float)(1.f / dt);

	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);
	if (Application::IsKeyPressed('B'))
	{
		std::cout << "hi" << std::endl;
	}

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

		bonuscount = 0;

		if (isrunning == false && balls >0 && b_allBallsdespawned == true)
		{
			isrunning = true;
		}


		EstimatedTime = -1;
		TimeTaken = 0;
		timerStarted = true;

	}
	while (balls >= 0 && isrunning == true)
	{
		float ranvalx = Math::RandFloatMinMax(-400, 300);
		float ranvaly = Math::RandFloatMinMax(750, 950);
		GameObject * ball = FetchGO(GameObject::GO_BALL);
		//ball->type = GameObject::GO_BALL;
		//ball->pos.Set(0, 0, 0);// = m_ghost->pos;
		ball->pos.Set(ranvalx, ranvaly, 0);
		ball->vel.Set(0, 0, 0);
		ball->scale.Set(27, 27, 27);
		ball->mass = 1.f;

		balls--;
		if (balls <= 0)
		{
			isrunning = false;
		}

	}
	if (isrunning == false && bonuscount > 0)
	{
		balls = bonuscount;
	}

	/*if (dt >= temp)
	{
	GameObject * ball = FetchGO(GameObject::GO_BALL);
	ball->vel.Set(0, 0, 0);
	ball->scale.Set(1.5, 1.5, 1.5);
	ball->mass = 1.f;
	}*/
	whitebang.Update(dt);
	bluebang.Update(dt);
	redbang.Update(dt);
	yellowbang.Update(dt);
	greenbang.Update(dt);

	b_allBallsdespawned = true;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end();)
	{
		if ((*it)->type == GameObject::GO_BALL && (*it)->b_isActive == true)
		{
			b_allBallsdespawned = false;
		}
		GameObject *go = (GameObject *)*it;
		if (go->b_isActive)
		{
			if (go->type == GameObject::GO_BALL)
			{
				go->vel += m_gravity;
				go->pos += go->vel * dt * m_speed;
				go->vel *= 0.98;

				float radius = go->scale.x;

				/*if ((go->pos.x > m_worldWidth - radius && go->vel.x > 0) ||
				(go->pos.x < radius && go->vel.x < 0))
				go->vel.x *= -1;*/

				if ((go->pos.y > 1000) ||
					(go->pos.y < -600))
				{
					if (go->pos.x >= -100 && go->pos.x <= 100)
					{
						bonuscount++;

						whitebang.SetType(GEO_PARTICLE_WHITE);
						//whitebang.pos.Set(go->pos.x, go->pos.y, go->pos.z);
						whitebang.SetFrequency(1);
						whitebang.SetCap(1000);
						whitebang.i_spawnAmount = 30;
						whitebang.f_lifeTime = 15.f;
						whitebang.minVel.Set(-10, 5, 0);
						whitebang.maxVel.Set(10, 100, 0);
						whitebang.scale.Set(10, 10, 10);
						whitebang.f_maxDist = 100.f;
						whitebang.isActive = false;
						whitebang.SpawnParticle(Vector3(go->pos.x, go->pos.y, 10));
					}

					go->b_isActive = false;
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


	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(1, false);
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

	int spacing = 13;

	RenderManager::GetInstance()->RenderTextOnScreen("No. of Balls: ", Color(1, 1, 1), 2, 0, 40);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(balls), Color(1, 1, 1), 2, spacing, 40);


	RenderManager::GetInstance()->RenderTextOnScreen("Blue Items: ", Color(0, 0.5, 1), 2, 0, 35);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource1), Color(0, 0.5, 1), 2, spacing, 35);

	RenderManager::GetInstance()->RenderTextOnScreen("Red Items: ", Color(1, 0, 0), 2, 0, 20);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource2), Color(1, 0, 0), 2, spacing, 20);

	RenderManager::GetInstance()->RenderTextOnScreen("YellowItems: ", Color(1, 1, 0), 2, 0, 30);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource3), Color(1, 1, 0), 2, spacing, 30);

	RenderManager::GetInstance()->RenderTextOnScreen("Green Items: ", Color(0, 0.7, 0), 2, 0, 25);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(resource4), Color(0, 0.7, 0), 2, spacing, 25);

	RenderManager::GetInstance()->RenderTextOnScreen("FPS: ", Color(1, 1, 1), 2, 0, 15);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(fps), Color(1, 1, 1), 2, spacing, 15);


	RenderManager::GetInstance()->RenderTextOnScreen("BonusCount: ", Color(1, 1, 1), 2, 0, 45);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(bonuscount), Color(1, 1, 1), 2, spacing, 45);

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
		go->pos.Set(0, 0, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		//go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}

	if (type == GameObject::GO_RESOURCE1)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE1;
		go->meshID = GEO_BLUECUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_RESOURCE2)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE2;
		go->meshID = GEO_REDCUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_RESOURCE3)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE3;
		go->meshID = GEO_YELLOWCUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_RESOURCE4)
	{
		go = new GameObject();
		go->type = GameObject::GO_RESOURCE4;
		go->meshID = GEO_GREENCUBE;
		go->pos.Set(0, -300, 0);
		go->scale.Set(105, 15, 15);
		go->b_isActive = true;
		go->normal.Set(0, 1, 0);
		go->rotation.Set(0, 0, 90);
		m_goList.push_back(go);
		m_objectCount++;
	}
	if (type == GameObject::GO_BONUSPAD)
	{
		go = new GameObject();
		go->type = GameObject::GO_BONUSPAD;
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
		Vector3 b1 = go->pos;
		Vector3 N = other->normal;

		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
		{
			N = -N;
		}
		float r = go->scale.x / 2;
		float h = other->scale.x / 2;
		float l = other->scale.y / 2;
		Vector3 NP = Vector3(-N.y, N.x);

		return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_RESOURCE1:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos;
		Vector3 N = other->normal;

		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
		{
			N = -N;
		}
		float r = go->scale.x / 2;
		float h = other->scale.x / 2;
		float l = other->scale.y / 2;
		Vector3 NP = Vector3(-N.y, N.x);


		return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_RESOURCE2:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos;
		Vector3 N = other->normal;

		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
		{
			N = -N;
		}
		float r = go->scale.x / 2;
		float h = other->scale.x / 2;
		float l = other->scale.y / 2;
		Vector3 NP = Vector3(-N.y, N.x);


		return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_RESOURCE3:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos;
		Vector3 N = other->normal;

		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
		{
			N = -N;
		}
		float r = go->scale.x / 2;
		float h = other->scale.x / 2;
		float l = other->scale.y / 2;
		Vector3 NP = Vector3(-N.y, N.x);



		return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_RESOURCE4:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos;
		Vector3 N = other->normal;

		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
		{
			N = -N;
		}
		float r = go->scale.x / 2;
		float h = other->scale.x / 2;
		float l = other->scale.y / 2;
		Vector3 NP = Vector3(-N.y, N.x);



		return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l *0.5f && go->vel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_BONUSPAD:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos;
		Vector3 N = other->normal;

		Vector3 dir = w0 - b1;
		if (dir.Dot(N) < 0)
		{
			N = -N;
		}
		float r = go->scale.x / 2;
		float h = other->scale.x / 2;
		float l = other->scale.y / 2;
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
		u = go->vel * 0.80;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		break;

	case GameObject::GO_RESOURCE1:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		resource1++;

		bluebang.SetType(GEO_PARTICLE_BLUE);
		bluebang.SetFrequency(1);
		bluebang.SetCap(1000);
		bluebang.i_spawnAmount = 30;
		bluebang.f_lifeTime = 5.f;
		bluebang.minVel.Set(-900, -900, 0);
		bluebang.maxVel.Set(900, 900, 0);
		bluebang.scale.Set(3, 3, 3);
		bluebang.f_maxDist = 100.f;
		bluebang.isActive = false;
		bluebang.SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));

		break;
	case GameObject::GO_RESOURCE2:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		resource2++;

		redbang.SetType(GEO_PARTICLE_RED);
		redbang.SetFrequency(1);
		redbang.SetCap(1000);
		redbang.i_spawnAmount = 30;
		redbang.f_lifeTime = 5.f;
		redbang.minVel.Set(-900, -900, 0);
		redbang.maxVel.Set(900, 900, 0);
		redbang.scale.Set(3, 3, 3);
		redbang.f_maxDist = 100.f;
		redbang.isActive = false;
		redbang.SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));

		break;
	case GameObject::GO_RESOURCE3:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;
		resource3++;

		yellowbang.SetType(GEO_PARTICLE_YELLOW);
		yellowbang.SetFrequency(1);
		yellowbang.SetCap(1000);
		yellowbang.i_spawnAmount = 30;
		yellowbang.f_lifeTime = 5.f;
		yellowbang.minVel.Set(-900, -900, 0);
		yellowbang.maxVel.Set(900, 900, 0);
		yellowbang.scale.Set(3, 3, 3);
		yellowbang.f_maxDist = 100.f;
		yellowbang.isActive = false;
		yellowbang.SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));

		break;
	case GameObject::GO_RESOURCE4:
		u = go->vel * 1.2;
		N = other->normal;
		uN = u.Dot(N)*N;
		go->vel = u - 2 * uN;

		resource4++;

		greenbang.SetType(GEO_PARTICLE_GREEN);
		greenbang.SetFrequency(1);
		greenbang.SetCap(1000);
		greenbang.i_spawnAmount = 30;
		greenbang.f_lifeTime = 5.f;
		greenbang.minVel.Set(-900, -900, 0);
		greenbang.maxVel.Set(900, 900, 0);
		greenbang.scale.Set(3, 3, 3);
		greenbang.f_maxDist = 100.f;
		greenbang.isActive = false;
		greenbang.SpawnParticle(Vector3(other->pos.x, other->pos.y, 10));

		break;
	case GameObject::GO_BONUSPAD:
		u = go->vel * 1.2;
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
//		ren_wall.meshID = GEO_BLUECUBE;
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
	Mtx44 rotate;

	grass.meshID = GEO_DUNGEONWALL;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(2000, 2000, 2000);
	grass.rotation.Set(-90, 0, 0);
	grass.rotation.Set(0, 0, 0);

	forValor.meshID = GEO_FOR_VALOR;
	forValor.pos.Set(0, 0.5, 0);
	forValor.scale.Set(2000, 2000, 2000);
	forValor.rotation.Set(-90, 0, 0);
	forValor.rotation.Set(0, 0, 0);
	


	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i % 2 == 0)
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE3);
				wall->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE3);
				wall2->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
			}
			else
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE1);
				wall->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE1);
				wall2->pos.Set(-370 + (i * 150), 250 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
			}
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (i % 2 == 0)
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE2);
				wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE2);
				wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
			}
			else
			{
				GameObject* wall = FetchGO(GameObject::GO_RESOURCE4);
				wall->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall->scale.Set(30, 30, 15);
				wall->rotation.Set(0, 0, 55);
				rotate.SetToIdentity();
				rotate.SetToRotation(-55, 0, 0, 1);
				wall->normal = rotate* wall->normal;
				wall->b_lightEnabled = false;

				GameObject* wall2 = FetchGO(GameObject::GO_RESOURCE4);
				wall2->pos.Set(-300 + (i * 150), 100 + (j * -300), 0);
				wall2->scale.Set(30, 30, 15);
				wall2->rotation.Set(0, 0, -55);
				rotate.SetToIdentity();
				rotate.SetToRotation(55, 0, 0, 1);
				wall2->normal = rotate* wall2->normal;
				wall2->b_lightEnabled = false;
			}
		}
	}

	//left right bounds
	GameObject* wall = FetchGO(GameObject::GO_WALL);
	wall->pos.Set(500, 0, 0);
	wall->scale.Set(50, 2000, 15);
	wall->normal.Set(1, 0, 0);
	wall->rotation.Set(0, 0, -180);

	GameObject* wall2 = FetchGO(GameObject::GO_WALL);
	wall2->pos.Set(-500, 0, 0);
	wall2->scale.Set(50, 2000, 15);
	wall2->normal.Set(1, 0, 0);
	wall2->rotation.Set(0, 0, -180);

	//bottom divider

	GameObject* pad = FetchGO(GameObject::GO_WALL);
	pad->pos.Set(0, -620, 0);
	pad->scale.Set(200, 50, 15);
	pad->rotation.Set(0, 0, 0);

	GameObject* divider = FetchGO(GameObject::GO_WALL);
	divider->pos.Set(-100, -620, 0);
	divider->scale.Set(200, 20, 15);
	divider->rotation.Set(0, 0, -55);
	rotate.SetToIdentity();
	rotate.SetToRotation(55, 0, 0, 1);
	divider->normal = rotate* divider->normal;

	GameObject* divider2 = FetchGO(GameObject::GO_WALL);
	divider2->pos.Set(100, -620, 0);
	divider2->scale.Set(200, 20, 15);
	divider2->rotation.Set(0, 0, 55);
	rotate.SetToIdentity();
	rotate.SetToRotation(-55, 0, 0, 1);
	divider2->normal = rotate* divider2->normal;







	//wall->rotation.Set(0, 0, 45);
	//Mtx44 rotate;
	//rotate.SetToRotation(-45, 0, 0, 1);
	//wall->normal = rotate* wall->normal;

	/*GameObject* wall2 = FetchGO(GameObject::GO_WALL);
	wall2->pos.Set(-230 + (i * 150), 50 + (j * -300), 0);
	wall2->scale.Set(30, 30, 15);
	wall2->rotation.Set(0, 0, -45);
	rotate.SetToIdentity();
	rotate.SetToRotation(45, 0, 0, 1);
	wall2->normal = rotate* wall2->normal;*/



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
