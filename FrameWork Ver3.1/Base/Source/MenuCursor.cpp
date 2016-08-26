#include "MenuCursor.h"
#include "Application.h"
#include "MathUtility.h"
#include "GUI.h"
#include "SceneManager.h"

MenuCursor::MenuCursor()
{
	bLButtonState = false;
}

MenuCursor::~MenuCursor()
{
	delete btn_play;

	delete btn_editor;

	delete btn_instructions;

	delete btn_option;

	delete btn_exit;

	delete instr_txt;
}

void MenuCursor::Init(GameplayCam *camera)
{
	this->camera = camera;
	movenorth = false;
	moveleft = false;
	moveright = false;

	istransition = false;
	menu_states = 0;
	scene_change = 1;

	MainButtonsInit();
	InstructionsInit();
	OptionsInit();
	//ButtonManager();
}

void MenuCursor::Update(const double &dt)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	worldX = x / w - 0.5f; // -0.5 - 0.5
	worldY = 1.f - (y / h) - 0.5f; // -0.5 - 0.5

	static const float CAMERA_SPEED = 60.f;

	if (menu_states == 3)
		SceneMoveRight(CAMERA_SPEED, dt);

	if (menu_states == 2)
		SceneMoveNorth(CAMERA_SPEED, dt);

	if (menu_states == 1)
		SceneMoveLeft(CAMERA_SPEED, dt);

	if (menu_states == 0)
		SceneMoveBack(CAMERA_SPEED, dt);


	if (istransition)
		ButtonDeactivator();
	else
	{
		//MainButtonRender();
		//InstructionsRender();
		ButtonManager();
	}

	
	Clicking(dt);
	HotKeys();
}

void MenuCursor::HotKeys()
{

}

void MenuCursor::Clicking(double dt)
{
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;

		GUI* temp = GUIManager::GetInstance()->FindGUI(worldX, worldY);
		if (temp != nullptr)
		{
			static const float CAMERA_SPEED = 10.f;

			if (temp->functionID == 0)
			{
				std::cout << "BUTTON PLAY" << std::endl;
				menu_states = 1;
				scene_change = 1;
				istransition = true;
			}
			if (temp->functionID == 1)
			{
				std::cout << "BUTTON EDITING" << std::endl;
				menu_states = 1;
				scene_change = 4;
				istransition = true;
			}
			if (temp->functionID == 2)
			{
				std::cout << "BUTTON TUTORIAL" << std::endl;
				menu_states = 2;
				istransition = true;
			}
			if (temp->functionID == 3)
			{
				std::cout << "BUTTON OPTIONS" << std::endl;
				menu_states = 3;
				istransition = true;
			}
			if (temp->functionID == 4)
			{
				std::cout << "BUTTON EXIT" << std::endl;
				menu_states = 1;
				istransition = true;
			}


		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		//istransition = true;
	}

	if (Application::IsMousePressed(1))
	{
		movenorth = false;
		moveleft = false;
		moveright = false;

		istransition = true;
		menu_states = 0;
	}
}

void MenuCursor::SceneMoveNorth(float cam_spd, double dt)
{
	float turnCam_spd = cam_spd + 30;

	if (camera->position.z >= -90)
	{
		Vector3 view = (camera->target - camera->position).Normalized();
		camera->position += view * turnCam_spd * (float)dt;
		camera->target += view * turnCam_spd * (float)dt;
	}
	else
		istransition = false;
		
}

void MenuCursor::SceneMoveRight(float cam_spd, double dt)
{
	float turnCam_spd = cam_spd + 30;

	if (camera->position.x <= 55)
	{
		Vector3 view = (camera->target - camera->position).Normalized();
		Vector3 right = view.Cross(camera->up);
		right.y = 0;
		right.Normalize();
		camera->position += right * turnCam_spd * (float)dt;
		camera->target += right * turnCam_spd * (float)dt;
	}
	else
		istransition = false;
}

void MenuCursor::SceneMoveLeft(float cam_spd, double dt)
{
	float turnCam_spd = cam_spd + 30;

	if (camera->position.x >= -30)
	{
		Vector3 view = (camera->target - camera->position).Normalized();
		Vector3 right = view.Cross(camera->up);
		right.y = 0;
		right.Normalize();
		camera->position -= right * turnCam_spd * (float)dt;
		camera->target -= right * turnCam_spd * (float)dt;
	}
	else
		istransition = false;
}

void MenuCursor::SceneMoveBack(float cam_spd, double dt)
{
	float turnCam_spd = cam_spd + 30;

	//return south
	if (camera->position.z <= 10)
	{
		Vector3 view = (camera->target - camera->position).Normalized();
		camera->position -= view * turnCam_spd * (float)dt;
		camera->target -= view * turnCam_spd * (float)dt;
	}
	
	else if (camera->position.x < -1)
	{
		Vector3 view = (camera->target - camera->position).Normalized();
		Vector3 right = view.Cross(camera->up);
		right.y = 0;
		right.Normalize();
		camera->position += right * turnCam_spd * (float)dt;
		camera->target += right * turnCam_spd * (float)dt;
	}
	else if (camera->position.x > 1)
	{
		Vector3 view = (camera->target - camera->position).Normalized();
		Vector3 right = view.Cross(camera->up);
		right.y = 0;
		right.Normalize();
		camera->position -= right * turnCam_spd * (float)dt;
		camera->target -= right * turnCam_spd * (float)dt;
	}
	else
	{
		istransition = false;
	}
}

void MenuCursor::ButtonManager()
{
	if (menu_states == 0)
		MainButtonRender();
	else if (menu_states == 2)
		InstructionsRender();
	else if (menu_states == 3)
		OptionsRender();
	else if (menu_states == 1)
		SceneManager::GetInstance()->ChangeScene(scene_change, false);
}

void MenuCursor::MainButtonsInit()
{
	
	btn_play = new GUI("Start Game");
	btn_play->position.Set(3, 30);
	btn_play->SetTextSize(3);
	btn_play->buttonSize.Set(15, 5);
	btn_play->functionID = 0;

	btn_editor = new GUI("Level Editor");
	btn_editor->position.Set(3, 25);
	btn_editor->SetTextSize(3);
	btn_editor->buttonSize.Set(20, 5);
	btn_editor->functionID = 1;

	btn_instructions = new GUI("Instructions");
	btn_instructions->position.Set(3, 20);
	btn_instructions->SetTextSize(3);
	btn_instructions->buttonSize.Set(20, 5);
	btn_instructions->functionID = 2;

	btn_option = new GUI("Options");
	btn_option->position.Set(3, 15);
	btn_option->SetTextSize(3);
	btn_option->buttonSize.Set(10, 5);
	btn_option->functionID = 3;

	btn_exit = new GUI("Exit");
	btn_exit->position.Set(3, 10);
	btn_exit->SetTextSize(3);
	btn_exit->buttonSize.Set(5, 5);
	btn_exit->functionID = 4;

	
}

void MenuCursor::MainButtonRender()
{
	btn_play->b_isActive = true;
	btn_play->b_textActive = true;

	btn_editor->b_isActive = true;
	btn_editor->b_textActive = true;

	btn_instructions->b_isActive = true;
	btn_instructions->b_textActive = true;

	btn_option->b_isActive = true;
	btn_option->b_textActive = true;

	btn_exit->b_isActive = true;
	btn_exit->b_textActive = true;

	


}

void MenuCursor::InstructionsInit()
{
	instr_txt = new GUI("Just some Instructions...blahblahblah ");
	instr_txt->position.Set(3, 30);
	instr_txt->SetTextSize(3);
	instr_txt->b_isActive = false;
	instr_txt->b_textActive = false;
	//, Color(1, 0.7, 0.3), 3, 18, 30);
}

void MenuCursor::InstructionsRender()
{
	instr_txt->b_isActive = true;
	instr_txt->b_textActive = true;
}

void MenuCursor::OptionsInit()
{

	option_1 = new GUI("Some Options...");
	option_1->position.Set(3, 30);
	option_1->SetTextSize(3);
	option_1->buttonSize.Set(15, 5);
	option_1->functionID = 0;
	option_1->b_isActive = false;
	option_1->b_textActive = false;

	option_2 = new GUI("Some Options...");
	option_2->position.Set(3, 25);
	option_2->SetTextSize(3);
	option_2->buttonSize.Set(15, 5);
	option_2->functionID = 0;
	option_2->b_isActive = false;
	option_2->b_textActive = false;

	option_3 = new GUI("Some Options...");
	option_3->position.Set(3, 20);
	option_3->SetTextSize(3);
	option_3->buttonSize.Set(15, 5);
	option_3->functionID = 0;
	option_3->b_isActive = false;
	option_3->b_textActive = false;

	option_4 = new GUI("Some Options...");
	option_4->position.Set(30, 30);
	option_4->SetTextSize(3);
	option_4->buttonSize.Set(15, 5);
	option_4->functionID = 0;
	option_4->b_isActive = false;
	option_4->b_textActive = false;

	option_5 = new GUI("Some Options...");
	option_5->position.Set(30, 25);
	option_5->SetTextSize(3);
	option_5->buttonSize.Set(15, 5);
	option_5->functionID = 0;
	option_5->b_isActive = false;
	option_5->b_textActive = false;

	option_6 = new GUI("Some Options...");
	option_6->position.Set(30, 20);
	option_6->SetTextSize(3);
	option_6->buttonSize.Set(15, 5);
	option_6->functionID = 0;
	option_6->b_isActive = false;
	option_6->b_textActive = false;




}

void MenuCursor::OptionsRender()
{
	option_1->b_isActive = true;
	option_1->b_textActive = true;

	option_2->b_isActive = true;
	option_2->b_textActive = true;

	option_3->b_isActive = true;
	option_3->b_textActive = true;

	option_4->b_isActive = true;
	option_4->b_textActive = true;

	option_5->b_isActive = true;
	option_5->b_textActive = true;

	option_6->b_isActive = true;
	option_6->b_textActive = true;
}

void MenuCursor::ButtonDeactivator()
{
	btn_play->b_isActive = false;
	btn_play->b_textActive = false;

	btn_editor->b_isActive = false;
	btn_editor->b_textActive = false;

	btn_instructions->b_isActive = false;
	btn_instructions->b_textActive = false;

	btn_option->b_isActive = false;
	btn_option->b_textActive = false;

	btn_exit->b_isActive = false;
	btn_exit->b_textActive = false;




	instr_txt->b_isActive = false;
	instr_txt->b_textActive = false;





	option_1->b_isActive = false;
	option_1->b_textActive = false;

	option_2->b_isActive = false;
	option_2->b_textActive = false;

	option_3->b_isActive = false;
	option_3->b_textActive = false;

	option_4->b_isActive = false;
	option_4->b_textActive = false;

	option_5->b_isActive = false;
	option_5->b_textActive = false;

	option_6->b_isActive = false;
	option_6->b_textActive = false;
}

