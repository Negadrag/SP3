
#include "MainMenu.h"
#include "GL\glew.h"
#include "LoadHmap.h"

#include "shader.hpp"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SceneManager.h"




MainMenu::MainMenu() :Scene()
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Init()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	testx = testy = testz = 0;
	banner_forward = -45;
	banner_backward = 80;

	camera.Init(Vector3(0, 4, 13), Vector3(0, 4, 12), Vector3(0, 1, 0), 1);
	camera.b_ortho = false;
	camera.farPlane = 100000.f;
	RenderManager::GetInstance()->SetCamera(&camera);

	b_initScene = false;
	std::cout << "entered";
	
	CreateScene();
	

	cursor.Init(&camera);

}

void MainMenu::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	banner_forward += dt * 30;
	if (banner_forward >= 80)
		banner_forward = -45;
	banner_backward -= dt * 30;
	if (banner_backward <= -45)
		banner_backward = 80;

	cursor.Update(dt);


	fps = (float)(1.f / dt);

	/*redfall.Update(dt);


	redfall.SetType(GEO_PARTICLE_RED);
	redfall.SetFrequency(1);
	redfall.SetCap(1000);
	redfall.i_spawnAmount = 1;
	redfall.f_lifeTime = 15.f;
	redfall.minVel.Set(-800, -100, 0);
	redfall.maxVel.Set(800, -100, 0);
	redfall.scale.Set(10, 10, 10);
	redfall.f_maxDist = 1500.f;
	redfall.isActive = false;
	redfall.SpawnParticle(Vector3(0, 1000, 1));*/


	if (Application::IsKeyPressed('L'))
		testx += dt * 50;
	if (Application::IsKeyPressed('J'))
		testx -= dt * 50;
	if (Application::IsKeyPressed('I'))
		testy += dt * 50;
	if (Application::IsKeyPressed('K'))
		testy -= dt * 50;
	if (Application::IsKeyPressed('P'))
		testz += dt * 50;
	if (Application::IsKeyPressed('O'))
		testz -= dt * 50;

	camera.Update(dt);
	RenderManager::GetInstance()->SetCamera(&camera);

	float skyboxsize = 10000.f;


	if (Application::IsKeyPressed('M'))
	{
		SceneManager::GetInstance()->ChangeScene(1, false);
	}

}

void MainMenu::Render()
{
	int spacing = 13;

	//-45 , 80
	RenderManager::GetInstance()->RenderTextOnScreen("CAPTURE DEFENCE ", Color(0.7, 0.7, 1), 5, 18, 50);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(fps), Color(1, 1, 1), 2, 45, 55);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testx), Color(1, 1, 1), 2, 45, 15);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testy), Color(1, 1, 1), 2, 45, 10);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(testz), Color(1, 1, 1), 2, 45, 5);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.x), Color(1, 0, 0), 2, 65, 35);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.y), Color(1, 0, 0), 2, 65, 30);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.position.z), Color(1, 0, 0), 2, 65, 25);

	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.target.x), Color(1, 0, 0), 2, 65, 15);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.target.y), Color(1, 0, 0), 2, 65, 10);
	RenderManager::GetInstance()->RenderTextOnScreen(std::to_string(camera.target.z), Color(1, 0, 0), 2, 65, 5);


	
	
}


void MainMenu::CreateScene()
{
	Mtx44 rotate;
	grass.meshID = GEO_GRASS_DARKGREEN;
	grass.pos.Set(0, 0, 0);
	grass.scale.Set(200, 200, 200);
	grass.rotation.Set(-90, 0, 0);
	//grass.rotation.Set(0, 0, 0);
	grass.b_shadows = true;
	grass.b_lightEnabled = false;
	
	CreateSkybox();
	SceneDeco();
	ButtonManager();


}

void MainMenu::CreateSkybox()
{
	float skyboxsize = 10000.f;

	skytop.meshID = GEO_SKYTOP;
	skytop.pos.Set(0, 5000, 0);
	skytop.rotation.Set(90, 0, 0);
	skytop.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skytop.b_shadows = false;
	skytop.b_lightEnabled = false;

	skyfront.meshID = GEO_SKYFRONT;
	skyfront.rotation.Set(0, 90, 0);
	skyfront.pos.Set(-5000, 0, 0);
	skyfront.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyfront.b_shadows = false;
	skyfront.b_lightEnabled = false;


	skyright.meshID = GEO_SKYRIGHT;
	skyright.pos.Set(0, 0, -skyboxsize / 2 + 2.f);
	skyright.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyright.b_shadows = false;
	skyright.b_lightEnabled = false;

	skyleft.meshID = GEO_SKYLEFT;
	skyleft.rotation.Set(0, 180, 0);
	skyleft.pos.Set(0, 0, 5000);
	skyleft.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyleft.b_shadows = false;
	skyleft.b_lightEnabled = false;

	skyback.meshID = GEO_SKYBACK;
	skyback.rotation.Set(0, -90, 0);
	skyback.pos.Set(5000, 0, 0);
	skyback.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skyback.b_shadows = false;
	skyback.b_lightEnabled = false;

	skybottom.meshID = GEO_SKYBOTTOM;
	skybottom.rotation.Set(-90, 0, 0);
	skybottom.pos.Set(0, -5000, 0);
	skybottom.scale.Set(skyboxsize, skyboxsize, skyboxsize);
	skybottom.b_shadows = false;
	skybottom.b_lightEnabled = false;



}

void MainMenu::SceneDeco()
{
	
	//center
	demo_main.meshID = GEO_ICETOWER;
	demo_main.pos.Set(0, .1, 0);
	demo_main.rotation.Set(-90, 0, 0);
	demo_main.scale.Set(1, 1, 1);
	demo_main.b_shadows = true;
	demo_main.b_lightEnabled = true;

	//north
	demo_play.meshID = GEO_CANNONTOWER;
	demo_play.pos.Set(0, .1, -40);
	demo_play.rotation.Set(-90, 0, 0);
	demo_play.scale.Set(1, 1, 1);
	demo_play.b_shadows = true;
	demo_play.b_lightEnabled = false;

	//right
	demo_instr.meshID = GEO_ARROWTOWER;
	demo_instr.pos.Set(20, .1, 0);
	demo_instr.rotation.Set(-90, 0, 0);
	demo_instr.scale.Set(1, 1, 1);
	demo_instr.b_shadows = true;
	demo_instr.b_lightEnabled = true;

	//left
	demo_option.meshID = GEO_POISONTOWER;
	demo_option.pos.Set(-20, .1, 0);
	demo_option.rotation.Set(-90, 0, 0);
	demo_option.scale.Set(1, 1, 1);
	demo_option.b_shadows = true;
	demo_option.b_lightEnabled = true;




}

void MainMenu::ButtonManager()
{
	MainButtons();

}

void MainMenu::MainButtons()
{
	btn_play = new GUI("Start Game");
	btn_play->b_isActive = true;
	btn_play->b_textActive = true;
	btn_play->position.Set(3, 30);
	btn_play->SetTextSize(3);
	btn_play->buttonSize.Set(15, 5);
	btn_play->functionID = 0;

	btn_editor = new GUI("Level Editor");
	btn_editor->b_isActive = true;
	btn_editor->b_textActive = true;
	btn_editor->position.Set(3, 25);
	btn_editor->SetTextSize(3);
	btn_editor->buttonSize.Set(20, 5);
	btn_editor->functionID = 1;

	btn_instructions = new GUI("Instructions");
	btn_instructions->b_isActive = true;
	btn_instructions->b_textActive = true;
	btn_instructions->position.Set(3, 20);
	btn_instructions->SetTextSize(3);
	btn_instructions->buttonSize.Set(20, 5);
	btn_instructions->functionID = 2;

	btn_option = new GUI("Options");
	btn_option->b_isActive = true;
	btn_option->b_textActive = true;
	btn_option->position.Set(3, 15);
	btn_option->SetTextSize(3);
	btn_option->buttonSize.Set(10, 5);
	btn_option->functionID = 3;

	btn_exit = new GUI("Exit");
	btn_exit->b_isActive = true;
	btn_exit->b_textActive = true;
	btn_exit->position.Set(3, 10);
	btn_exit->SetTextSize(3);
	btn_exit->buttonSize.Set(5, 5);
	btn_exit->functionID = 4;
}

void MainMenu::OptionButtons()
{

}

void MainMenu::Exit()
{

	redfall.ClearParticles();
}
