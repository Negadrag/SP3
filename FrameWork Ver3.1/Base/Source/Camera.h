#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

struct AspectRatio
{
	AspectRatio(){};
	~AspectRatio(){};

	double x, y;

	void Set(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

};

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	bool b_ortho;

	float orthoSize;
	float defaultOrtho;
	AspectRatio aspectRatio;

	float FOV;
	float nearPlane;
	float farPlane;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif