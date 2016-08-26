#include "Cursor.h"
#include "Application.h"
#include "MathUtility.h"

Cursor::Cursor()
{
	worldCoords.SetZero();
	worldX = worldY = 0.f;
}

Cursor::~Cursor()
{
}

void Cursor::Update(OrthoCamera &camera, const TileMap &tileMap, const double &dt)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	worldX = x / w - 0.5f; // -0.5 - 0.5
	worldY = 1.f - (y / h) - 0.5f; // -0.5 - 0.5

	float worldY2 = worldY / cos(Math::DegreeToRadian(camera.rotation)); // Slanting the camera by getting the hypotenuse

	float Xunits = 0.5f / (camera.orthoSize * (camera.aspectRatio.x / camera.aspectRatio.y)); // 1 unit in world space
	float Yunits = 0.5f / (camera.orthoSize);

	Vector3 center = camera.target;

	worldCoords.Set(center.x + worldX / Xunits + 0.5f, center.y + worldY2 / Yunits);
}

void Cursor::EdgePanning(const double &dt, OrthoCamera &camera, float worldX, float worldY, float speed)
{
	if (worldX > 0.4)
	{
		camera.target.x += (worldX - 0.4f) * speed * (float)dt;
		camera.position.x += (worldX - 0.4f) * speed * (float)dt;
	}
	if (worldX < -0.4)
	{
		camera.target.x += (worldX + 0.4f) * speed * (float)dt;
		camera.position.x += (worldX + 0.4f) * speed * (float)dt;
	}

	if (worldY > 0.4)
	{
		camera.target.y += (worldY - 0.4f) * speed * (float)dt;
		camera.position.y += (worldY - 0.4f) * speed * (float)dt;
	}
	if (worldY < -0.4)
	{
		camera.target.y += (worldY + 0.4f) * speed * (float)dt;
		camera.position.y += (worldY + 0.4f) * speed * (float)dt;
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