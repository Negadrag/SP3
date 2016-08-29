#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "RenderManager.h"
#include "EntityManager.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	bool b_Exit;
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool GetMouseUpdate();
	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();

	static int mouse_scroll;
private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 30;
	const static int m_window_width = 1920;
	const static int m_window_height = 1080;
	//Declare a window object

	StopWatch m_timer;
};

#endif