#include "RenderManager.h"
#include "Renderable.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Utility.h"
#include "MeshBuilder.h"
#include "LoadHmap.h"
#include "LoadTGA.h"
#include "Application.h"

#include <iostream>

using namespace std;

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager()
{
	m_vertexArrayID = 0;
	list<Renderable*> temp;
	renderableList.push_back(temp);
}

RenderManager::~RenderManager()
{
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		delete meshList[i];
	}
}

void RenderManager::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
	glUseProgram(m_programID);
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");

	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	//Get a handle for shadows
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");

	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(-3, -3, 3);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.5f;
	lights[0].kC = 1.0f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.0f, 1.f, 0.0f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	Color fogColor(0.7f, 0.7f, 0.7f);

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);

	glUniform1f(m_parameters[U_FOG_START], 0.f);
	glUniform1f(m_parameters[U_FOG_END], 10.f);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.003f);

	glUniform1i(m_parameters[U_FOG_TYPE], 2);
	glUniform1i(m_parameters[U_FOG_ENABLED], 0);

	m_lightDepthFBO.Init(4096, 4096);
	
	InitMesh();

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);

}

void RenderManager::InitMesh()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_NULL] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureArray[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureArray[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 0.5f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	meshList[GEO_CONE] = MeshBuilder::GenerateSphere("cone", Color(0.5f, 1, 0.3f), 18, 36, 0.5f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	meshList[GEO_BLUECUBE] = MeshBuilder::GenerateCube("bluecube", Color(0, 0, 1), 1.f);
	meshList[GEO_REDCUBE] = MeshBuilder::GenerateCube("bluecube", Color(1, 0, 0), 1.f);
	meshList[GEO_YELLOWCUBE] = MeshBuilder::GenerateCube("bluecube", Color(1, 1, 0), 1.f);
	meshList[GEO_GREENCUBE] = MeshBuilder::GenerateCube("bluecube", Color(0, 0.7, 0), 1.f);

	meshList[GEO_DUNGEONWALL] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_DUNGEONWALL]->textureArray[0] = LoadTGA("Image//dungeonwall.tga");

	meshList[GEO_CUBE] = MeshBuilder::GenerateOBJ("Cube", "OBJ/Cube.obj");
	meshList[GEO_CUBE2] = MeshBuilder::GenerateOBJ("Cube", "OBJ/Cube.obj");
	meshList[GEO_CUBE2]->textureArray[0] = LoadTGA("Image//Face.tga");

	//load texture and mesh for skyplane
	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("skyplane", Color(1, 1, 1), 128, 500.0f, 300.f, 5.f, 5.f);
	meshList[GEO_SKYPLANE]->textureArray[0] = LoadTGA("Image//sky.tga");
	//meshList[GEO_SKYPLANE]->textureArray[1] = LoadTGA("Image//sunsetsky.tga");

	// Load the ground mesh and texture
	meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_DARKGREEN]->textureArray[0] = LoadTGA("Image//ground.tga");
	meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_LIGHTGREEN]->textureArray[0] = LoadTGA("Image//grass_lightgreen.tga");

	//for terrain
	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerain("Terrain", "Image//heightmap4.raw");
	meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//soil_texture.tga");
	meshList[GEO_TERRAIN]->textureArray[1] = LoadTGA("Image//grass2.tga");
	/*meshList[GEO_TERRAIN]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TERRAIN]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TERRAIN]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TERRAIN]->material.kShininess = 0.5f;*/
	meshList[GEO_TERRAIN]->material.kDiffuse.Set(0.95f, 0.95f, 0.95f);
	meshList[GEO_TERRAIN]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TERRAIN]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TERRAIN]->material.kShininess = 1.0f;

	//Monster
	meshList[GEO_BASIC] = MeshBuilder::GenerateOBJ("Basic", "OBJ/BasicMonster.obj");
	meshList[GEO_BASIC]->textureArray[0] = LoadTGA("Image//BasicMonster.tga");
	/*meshList[GEO_BASIC]->material.kShininess = 0.8f;
	meshList[GEO_BASIC]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);*/
	//meshList[GEO_SPEED] = MeshBuilder::GenerateOBJ("Speed", "OBJ/SpeedMonster.obj");
	//meshList[GEO_SPEED]->textureArray[0] = LoadTGA("Image//SpeedMonster.tga");
	meshList[GEO_TANKY] = MeshBuilder::GenerateOBJ("Tanky", "OBJ/TankyMonster.obj");
	meshList[GEO_TANKY]->textureArray[0] = LoadTGA("Image//TankyMonster.tga");
	
	meshList[GEO_PATH] = MeshBuilder::GenerateQuad("Path", Color(1, 1, 1), 1.f);
	meshList[GEO_PATH]->textureArray[0] = LoadTGA("Image//Path.tga");

	meshList[GEO_FOR_VALOR] = MeshBuilder::GenerateQuad("FOR VALOR", Color(1, 1, 1), 1.f);
	meshList[GEO_FOR_VALOR]->textureArray[0] = LoadTGA("Image//Valor.tga");				

	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();


	//Tower
	meshList[GEO_ARROWTOWER] = MeshBuilder::GenerateOBJ("Arrowtower", "OBJ//Tower-ARROW.obj");
	meshList[GEO_ARROWTOWER]->textureArray[0] = LoadTGA("Image//Tower-ARROW.tga");
	meshList[GEO_POISONTOWER] = MeshBuilder::GenerateOBJ("Arrowtower", "OBJ//Tower-ARROW+BASIC.obj");
	meshList[GEO_POISONTOWER]->textureArray[0] = LoadTGA("Image//Tower-ARROW+BASIC.tga");
	meshList[GEO_ARROW] = MeshBuilder::GenerateOBJ("Arrowtower", "OBJ//Arrow.obj");
	meshList[GEO_CANNONTOWER] = MeshBuilder::GenerateOBJ("Arrowtower", "OBJ//Tower-CANNON.obj");
	meshList[GEO_CANNONTOWER]->textureArray[0] = LoadTGA("Image//Tower-CANNON.tga");
	meshList[GEO_CANNON] = MeshBuilder::GenerateSphere("cannon", Color(0, 0, 0), 18, 36, 0.5f);
	

	//Particles
	meshList[GEO_SMOKEPARTICLES] = MeshBuilder::GenerateQuad("smoke particle", Color(1, 1, 1), 1.f);
	meshList[GEO_SMOKEPARTICLES]->textureArray[0] = LoadTGA("Image//smokeParticle.tga");

	meshList[GEO_PARTICLE_WHITE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_PARTICLE_BLUE] = MeshBuilder::GenerateSphere("sphere", Color(0, 0.6, 1), 18, 36, 1.f);
	meshList[GEO_PARTICLE_RED] = MeshBuilder::GenerateSphere("sphere", Color(1, 0.3, 0.3), 18, 36, 1.f);
	meshList[GEO_PARTICLE_YELLOW] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 0), 18, 36, 1.f);
	meshList[GEO_PARTICLE_GREEN] = MeshBuilder::GenerateSphere("sphere", Color(0, 0.7, 0), 18, 36, 1.f);

	
}

RenderManager* RenderManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new RenderManager();
	}
	return instance;
}


void RenderManager::RenderObj(Renderable* obj) 
{
	
	if (obj->scale.IsZero()) {
		return;
	}
	if (obj->meshID == GEO_NULL)
	{
		return;
	}
	if (obj->GetParent() != nullptr)
	{
		Renderable* parent = obj->GetParent();
		modelStack.PushMatrix();
		modelStack.Translate(parent->pos.x, parent->pos.y, parent->pos.z);

		modelStack.Rotate(parent->rotation.z, 0, 0, 1);
		modelStack.Rotate(parent->rotation.x, 1, 0, 0);
		modelStack.Rotate(parent->rotation.y, 0, 1, 0);
		

		modelStack.Scale(parent->scale.x, parent->scale.y, parent->scale.z);
	}
	modelStack.PushMatrix();
		modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
		if (obj->b_3DbillBoard == true) {
			Vector3 dist = camera->position - obj->pos;
			Vector3 forward = Vector3(0, 0, 1);
			Vector3 axisOfRotation = forward.Cross(dist);
			float angleOfRotation = Math::RadianToDegree(acos(forward.Dot(Vector3(dist)) / dist.Length()));
			modelStack.Rotate(angleOfRotation, axisOfRotation.x, axisOfRotation.y, axisOfRotation.z);
		} else {
			modelStack.Rotate(obj->rotation.z, 0, 0, 1);
			modelStack.Rotate(obj->rotation.x, 1, 0, 0);
			modelStack.Rotate(obj->rotation.y, 0, 1, 0);
		}
		modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
		RenderMesh(obj->meshID, obj->b_lightEnabled , obj->b_fog);
	modelStack.PopMatrix();
	if (obj->GetParent() != nullptr)
	{
		modelStack.PopMatrix();
	}
}


void RenderManager::RenderGPass(int sceneID)
{
	if (sceneID == 0)
	{
		return;
	}
	m_renderPass = RENDER_PASS_PRE;
	
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	
	viewStack.LoadIdentity();
	modelStack.LoadIdentity();
	projectionStack.LoadIdentity();

	//These matrices should change when light position or direction changes
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		float width = camera->orthoSize * camera->aspectRatio.x / camera->aspectRatio.y;
		m_lightDepthProj.SetToOrtho(-width*2,width*2,-camera->orthoSize*2, camera->orthoSize*2, -camera->farPlane*2, camera->farPlane*2);
	}
	else
	{
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}
	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	/*
	for (list<Renderable*>::iterator it = renderableList[sceneID].begin(); it != renderableList[sceneID].end(); ++it)
	{
		if ((*it)->b_isActive == true && (*it)->b_Render == true && (*it)->b_shadows == true)
		{
			RenderObj((*it));
		}
	}*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderManager::RenderMain(int sceneID)
{
	if (sceneID == 0)
	{
		return;
	}
	m_renderPass = RENDER_PASS_MAIN;

	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera->position.x, camera->position.y, camera->position.z,
		camera->target.x, camera->target.y, camera->target.z,
		camera->up.x, camera->up.y, camera->up.z
	);
	projectionStack.LoadIdentity();
	Mtx44 perspective;
	if (camera->b_ortho == false)
	{
		perspective.SetToPerspective(camera->FOV, camera->aspectRatio.x / camera->aspectRatio.y, camera->nearPlane, camera->farPlane);

	}
	else
	{
		float width = camera->orthoSize * camera->aspectRatio.x / camera->aspectRatio.y;
		perspective.SetToOrtho(-width, width, -camera->orthoSize, camera->orthoSize, camera->nearPlane, camera->farPlane);
	}
	projectionStack.LoadMatrix(perspective);
	
	for (unsigned int n = 0; n < sizeof(lights)/sizeof(*lights); ++n) {
		glUniform1i(m_parameters[U_LIGHT0_TYPE + n], lights[n].type);
		if (lights[n].type == Light::LIGHT_DIRECTIONAL) {
			Vector3 lightDir(lights[n].position.x, lights[n].position.y, lights[n].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + n], 1, &lightDirection_cameraspace.x);
		} else if (lights[n].type == Light::LIGHT_SPOT) {
			Position lightPosition_cameraspace = viewStack.Top() * lights[n].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + n], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[n].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION + n], 1, &spotDirection_cameraspace.x);
		} else { //Point Light
			Position lightPosition_cameraspace = viewStack.Top() * lights[n].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + n], 1, &lightPosition_cameraspace.x);
		}
	}
	
	glUniform1i(m_parameters[U_FOG_ENABLED], 0);

	for (list<Renderable*>::iterator it = renderableList[sceneID].begin(); it != renderableList[sceneID].end(); ++it)
	{
			if ((*it)->b_isActive == true && (*it)->b_Render == true)
			{
				RenderObj((*it));
			}
	}

}

void RenderManager::RenderMesh(GEOMETRY_TYPE meshID, bool enableLight,bool fog) {

	Mesh* mesh = meshList[meshID];
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	if (m_renderPass == RENDER_PASS_PRE) {
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

		for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
			if (mesh->textureArray[i] > 0) {
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
			} else {
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
			}
		}
		mesh->Render();

		for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
			if (mesh->textureArray[i] > 0) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		return;
	}
	if (fog == false)
	{
		glUniform1i(m_parameters[U_FOG_ENABLED], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_FOG_ENABLED], 1);
	}

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

	if (enableLight) {
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		//Load Material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	} else {
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
		if (mesh->textureArray[i] > 0) {
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		} else {
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}

	mesh->Render();

	for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
		if (mesh->textureArray[i] > 0) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

}


void RenderManager::RenderMesh(GEOMETRY_TYPE meshID, Vector3 pos, Vector3 scale, Vector3 rotation, bool enableLight, bool fog) {

	Mesh* mesh = meshList[meshID];
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	modelStack.PushMatrix();
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Rotate(rotation.x, 1, 0, 0);
	modelStack.Rotate(rotation.z, 0, 0, 1);
	modelStack.Rotate(rotation.y, 0, 1, 0);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (m_renderPass == RENDER_PASS_PRE) {
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

		for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
			if (mesh->textureArray[i] > 0) {
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
			}
			else {
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
			}
		}
		mesh->Render();

		for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
			if (mesh->textureArray[i] > 0) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		return;
	}
	if (fog == false)
	{
		glUniform1i(m_parameters[U_FOG_ENABLED], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_FOG_ENABLED], 1);
	}

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

	if (enableLight) {
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		//Load Material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else {
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
		if (mesh->textureArray[i] > 0) {
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else {
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}

	mesh->Render();

	for (unsigned int i = 0; i < Mesh::MAX_TEXTURES; ++i) {
		if (mesh->textureArray[i] > 0) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	modelStack.PopMatrix();

}

void Swap(Renderable* obj, Renderable* obj2)
{
	Renderable* temp = obj;
	obj = obj2;
	obj2 = temp;
}

void RenderManager::Update(double dt)
{
	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);	
	
	if (Application::IsKeyPressed('B'))
	{
		viewStack.LoadIdentity();
	}
	//lights[0].position.Set(-6, 30, 300);
	// Camera matrix	
}

void RenderManager::AddRenderable(Renderable* entity)
{
	if (entity->m_sceneID >= renderableList.size())
	{
		list<Renderable*> newList;
		newList.push_back(entity);
		renderableList.push_back(newList);
	}
	this->renderableList[entity->m_sceneID].push_back(entity);
}

void RenderManager::RemoveRenderable(Renderable* entity)
{
	this->renderableList[entity->m_sceneID].remove(entity);
}

void RenderManager::SetCamera(Camera* cam)
{
	this->camera = cam;
}

void RenderManager::UpdateBillboard(int sceneID)
{
	if (sceneID == 0)
	{
		return;
	}
	for (list<Renderable*>::iterator it = renderableList[sceneID].begin(); it != renderableList[sceneID].end(); ++it)
	{
		if ((*it)->b_Render == true)
		{
			(*it)->UpdateBillboard();
		}
	}
}

void RenderManager::RenderTextOnScreen(std::string text, Color color, float size, float x, float y)
{
	Mesh *mesh = meshList[GEO_TEXT];

	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	//modelStack.Translate(0, 0, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	//cout << "Text enablked" << m_parameters[U_TEXT_ENABLED] << endl;
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
		//mesh->Render();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::RenderMeshOnScreen(GEOMETRY_TYPE geo, bool lightEnabled, float size, float x, float y)
{
	Mesh* mesh = meshList[geo];
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -50, 50);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	//modelStack.Rotate(15, 0, 1, 0);
	//modelStack.Rotate(-70, 1, 0, 0);
	modelStack.Scale(size, size, size);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	if (lightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureArray[0] > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureArray[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void RenderManager::RenderMeshOnScreen(GEOMETRY_TYPE geo, bool lightEnabled, Vector3 position, Vector3 scale, Vector3 rotation)
{
	Mesh* mesh = meshList[geo];
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -100, 100);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotation.z, 0, 0, 1);
	modelStack.Rotate(rotation.x, 1, 0, 0);
	modelStack.Rotate(rotation.y, 0, 1, 0);
	modelStack.Scale(scale.x, scale.y, scale.z);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	if (lightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureArray[0] > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureArray[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}