#include "GameplayCam.h"
#include "Application.h"
#include "Mtx44.h"

GameplayCam::GameplayCam()
{

}

GameplayCam::~GameplayCam()
{

}

void GameplayCam::Init(const Vector3& pos, const Vector3& target, Vector3& up)
{
	

	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	this->b_ortho = true;
	this->aspectRatio.Set(4, 3);
	this->FOV = 45.f;
	this->orthoSize = 750.f;
	this->nearPlane = 0.1;
	this->farPlane = 10000;

	leftright = false;
	turnSpeed_Mod = 0; 
	started = false;

}

void GameplayCam::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;
	

	//TurnTable(CAMERA_SPEED, dt, position);

	PanCamera(CAMERA_SPEED, dt);

}

void GameplayCam::TurnTable(float cam_spd, double dt, Vector3& targetpos)
{
	if (!started)
	{
		Vector3 temp(200, 340, 400);
		position = targetpos + temp;
		started = true;
		
	}

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

	float turnCam_spd = cam_spd + turnSpeed_Mod;

	if (Application::IsKeyPressed('A'))
		leftright = false;

	if (Application::IsKeyPressed('D'))
		leftright = true;


	if (!leftright)
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * turnCam_spd * (float)dt;
	}
	else
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * turnCam_spd * (float)dt;
	}
	
	//Camera Zoom
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		position += view * turnCam_spd * (float)dt;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * turnCam_spd * (float)dt;
	}


	//Camera Pitching
	//if (Application::IsKeyPressed(VK_UP))
	//{
	//	float pitch = (float)(-turnCam_spd * (float)dt);
	//	Vector3 view = (target - position).Normalized();
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	//target = position + view;
	//}
	//if (Application::IsKeyPressed(VK_DOWN))
	//{
	//	float pitch = (float)(turnCam_spd * (float)dt);
	//	Vector3 view = (target - position).Normalized();
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	//target = position + view;
	//}



	//Camera Turn Speed Modifier
	if (Application::IsKeyPressed(VK_LEFT))
	{
		if (turnSpeed_Mod <= 0)
			turnSpeed_Mod = 0;
		else
			turnSpeed_Mod -= 2;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		if (turnSpeed_Mod >= 1000)
			turnSpeed_Mod = 1000;
		else
			turnSpeed_Mod += 2;
	}
	std::cout << turnSpeed_Mod << std::endl;
}

void GameplayCam::PanCamera(float cam_spd, double dt)
{
	float turnCam_spd = cam_spd + turnSpeed_Mod;

	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * turnCam_spd * (float)dt;
		target -= right * turnCam_spd * (float)dt;
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * turnCam_spd * (float)dt;
		target += right * turnCam_spd * (float)dt;
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		position += view * turnCam_spd * (float)dt;
		target += view * turnCam_spd * (float)dt;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * turnCam_spd * (float)dt;
		target -= view * turnCam_spd * (float)dt;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(turnCam_spd * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-turnCam_spd * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(turnCam_spd * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-turnCam_spd * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	//Update the camera direction based on mouse move
	// left-right rotate
	/*{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-turnCam_spd * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	{
		float pitch = (float)(-turnCam_spd * Application::camera_pitch * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}*/
}

void GameplayCam::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}


