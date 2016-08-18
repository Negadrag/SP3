#include "OrthoCamera.h"
#include "Mtx44.h"

OrthoCamera::OrthoCamera()
{

}

OrthoCamera::~OrthoCamera()
{

}

void OrthoCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float &rotation)
{
	this->rotation = rotation;
	Mtx44 rotAngle;
	rotAngle.SetToRotation(rotation, 1, 0, 0);
	Vector3 tempos = rotAngle * pos;
	this->position = defaultPosition = tempos;
	this->target = defaultTarget = target;
	this->up = defaultUp = up;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.Normalize();
	this->b_ortho = false;
	this->aspectRatio.Set(4, 3);
	this->FOV = 45.f;
	this->orthoSize = 10.f;
	this->nearPlane = 0.1f;
	this->farPlane = 10000;
}

void OrthoCamera::Update(double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	//right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}

void OrthoCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}