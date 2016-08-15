#include "Scene5.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"


Scene5::Scene5()
{
}

Scene5::~Scene5()
{
}

void Scene5::Init()
{
	
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//TransformVertexShader.vertexshader", "Shader//ColorFragmentShader.fragmentshader" );
	
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	
	// Use our shader
	glUseProgram(m_programID);

	camera.Init(Vector3(0, 0, 200), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.5f, 0.5f, 0.5f), 1.f);
	meshList[GEO_CUBE1] = MeshBuilder::GenerateCube("cube1", Color(1, 1, 0), 1.f);
	meshList[GEO_CUBE2] = MeshBuilder::GenerateCube("cube2", Color(1, 0, 1), 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
}

void Scene5::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	static const float ROTATE_SPEED = 10.f;
	static int rotateDir = 1;
	if(rotateAngle * rotateDir > 30)
		rotateDir = -rotateDir;
	rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

	camera.Update(dt);
}
void Scene5::RenderBody()
{
	Mtx44 MVP;
	modelStack.PushMatrix();
	modelStack.Translate(0, -7.5f, 0);
	modelStack.Scale(30, 25, 10);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_CUBE1]->Render();
	modelStack.PopMatrix();
}
void Scene5::RenderLimb()
{
	Mtx44 MVP;
	modelStack.PushMatrix();
	modelStack.Translate(0, -5.f, 0);
	modelStack.Scale(10, 20, 10);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_CUBE2]->Render();
	modelStack.PopMatrix();
}

void Scene5::RenderRobot()
{
	Mtx44 MVP;

	modelStack.PushMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 15, 0);
	modelStack.Scale(10, 10, 10);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SPHERE]->Render();
	modelStack.PopMatrix();

	RenderBody();
	
	modelStack.PushMatrix();
	modelStack.Translate(20, 0, 0);
	modelStack.Rotate(rotateAngle, 1, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-20, 0, 0);
	modelStack.Rotate(-rotateAngle, 1, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-20, 0, 0);
	modelStack.Rotate(-rotateAngle, 1, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-10, -25, 0);
	modelStack.Rotate(rotateAngle, 1, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(30, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(10, -25, 0);
	modelStack.Rotate(-rotateAngle, 1, 0, 0);
	RenderLimb();
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	modelStack.Rotate(30, 1, 0, 0);
	RenderLimb();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Scene5::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 MVP;

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, -60, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_QUAD]->Render();
	modelStack.PopMatrix();
	
	for(int row = 0; row < 4; ++row)
	{
		for(int col = 0; col < 3; ++col)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-60.f + col * 60.f, 0, -60.f + row * 60.f);
			RenderRobot();
			modelStack.PopMatrix();
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene5::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
