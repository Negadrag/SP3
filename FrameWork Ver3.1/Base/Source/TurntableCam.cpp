#include "TurntableCam.h"
#include "Application.h"
#include "Mtx44.h"

TurntableCam::TurntableCam()
{

}

TurntableCam::~TurntableCam()
{

}

void TurntableCam::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.Normalize();
	this->b_ortho = false;
	this->aspectRatio.Set(4, 3);
	this->FOV = 45.f;
	this->orthoSize = 10.f;
	this->nearPlane = 0.1;
	this->farPlane = 10000;

	leftright = false;
}

void TurntableCam::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;

	if (!leftright)
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * CAMERA_SPEED * (float)dt;
	}
	else
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * CAMERA_SPEED * (float)dt;
	}

	if (Application::IsKeyPressed('A'))
	{
		leftright = false;
	}
	if (Application::IsKeyPressed('D'))
	{
		leftright = true;
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		position += view * CAMERA_SPEED * (float)dt;
		//target += view * CAMERA_SPEED * (float)dt;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * CAMERA_SPEED * (float)dt;
		//target -= view * CAMERA_SPEED * (float)dt;
	}
	

	
}

void TurntableCam::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}


