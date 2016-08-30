#include "Cursor.h"
#include "Application.h"
#include "MathUtility.h"

Cursor::Cursor()
{
	worldCoords.SetZero();
	screenX = screenY = 0.f;
}

Cursor::~Cursor()
{
}

void Cursor::Update(OrthoCamera &camera, const double &dt)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	screenX = x / w - 0.5f; // -0.5 - 0.5
	screenY = 1.f - (y / h) - 0.5f; // -0.5 - 0.5

	float screenY2 = screenY / cos(Math::DegreeToRadian(camera.rotation)); // Slanting the camera by getting the hypotenuse

	float Xunits = 0.5f / (camera.orthoSize * (camera.aspectRatio.x / camera.aspectRatio.y)); // 1 unit in world space
	float Yunits = 0.5f / (camera.orthoSize);

	Vector3 center = camera.target;

	worldCoords.Set(center.x + screenX / Xunits + 0.5f, center.y + screenY2 / Yunits);
}

void Cursor::EdgePanning(const double &dt, OrthoCamera &camera, float screenX, float screenY, float speed)
{
	if (screenX > 0.4)
	{
		camera.target.x += (screenX - 0.4f) * speed * (float)dt;
		camera.position.x += (screenX - 0.4f) * speed * (float)dt;
	}
	if (screenX < -0.4)
	{
		camera.target.x += (screenX + 0.4f) * speed * (float)dt;
		camera.position.x += (screenX + 0.4f) * speed * (float)dt;
	}

	if (screenY > 0.4)
	{
		camera.target.y += (screenY - 0.4f) * speed * (float)dt;
		camera.position.y += (screenY - 0.4f) * speed * (float)dt;
	}
	if (screenY < -0.4)
	{
		camera.target.y += (screenY + 0.4f) * speed * (float)dt;
		camera.position.y += (screenY + 0.4f) * speed * (float)dt;
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		camera.target.y += 0.1f * speed * (float)dt;
		camera.position.y += 0.1f * speed * (float)dt;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		camera.target.y -= 0.1f * speed * (float)dt;
		camera.position.y -= 0.1f * speed * (float)dt;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		camera.target.x -= 0.1f * speed * (float)dt;
		camera.position.x -= 0.1f * speed * (float)dt;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		camera.target.x += 0.1f * speed * (float)dt;
		camera.position.x += 0.1f * speed * (float)dt;
	}
}

void Cursor::CameraBounds(OrthoCamera &camera)
{
	if (camera.target.x > camera.defaultTarget.x + (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y))
	{
		float offset = camera.defaultTarget.x + (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y) - camera.target.x;
		camera.target.x += offset;
		camera.position.x += offset;
	}
	else if (camera.target.x < camera.defaultTarget.x - (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y))
	{
		float offset = camera.defaultTarget.x - (camera.defaultOrtho - camera.orthoSize) * (camera.aspectRatio.x / camera.aspectRatio.y) - camera.target.x;
		camera.target.x += offset;
		camera.position.x += offset;
	}

	if (camera.target.y > camera.defaultTarget.y + (camera.defaultOrtho - camera.orthoSize))
	{
		float offset = camera.defaultTarget.y + (camera.defaultOrtho - camera.orthoSize) - camera.target.y;
		camera.target.y += offset;
		camera.position.y += offset;
	}
	else if (camera.target.y < camera.defaultTarget.y - (camera.defaultOrtho - camera.orthoSize))
	{
		float offset = camera.defaultTarget.y - (camera.defaultOrtho - camera.orthoSize) - camera.target.y;
		camera.target.y += offset;
		camera.position.y += offset;
	}
}

void Cursor::Clear()
{
}