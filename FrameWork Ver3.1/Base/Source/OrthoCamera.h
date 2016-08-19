#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include "Camera3.h"

class OrthoCamera : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	float defaultOrtho;

	float rotation;

	OrthoCamera();
	~OrthoCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float &rotation = 0.f);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif