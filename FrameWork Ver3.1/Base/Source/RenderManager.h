#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <list>
#include <vector>

#include "Mesh.h"
#include "MatrixStack.h"
#include "Camera3.h"
#include "Light.h"
#include "DepthFBO.h"
#include <string>

using std::string;
using std::list;
using std::vector;

class Renderable;

enum GEOMETRY_TYPE
{
	GEO_NULL = 0,
	GEO_AXES,
	GEO_CROSSHAIR,
	GEO_LIGHTBALL,
	GEO_SPHERE,
	GEO_QUAD,
	GEO_CUBE,
	GEO_RING,
	GEO_CONE,
	GEO_LEFT,
	GEO_RIGHT,
	GEO_TOP,
	GEO_BOTTOM,
	GEO_FRONT,
	GEO_BACK,
	GEO_GRASS_DARKGREEN,
	GEO_GRASS_LIGHTGREEN,
	GEO_OBJECT,
	GEO_TEXT,
	GEO_TERRAIN,

	GEO_CUBE2,
	GEO_WATER,
	GEO_SMOKEPARTICLES,
	//TSL
	GEO_SKYPLANE,
	GEO_LIGHT_DEPTH_QUAD,

	//Tower
	GEO_ARROWTOWER,
	GEO_CANNONTOWER,
	GEO_ARROW,
	GEO_CANNON,

	//Monster
	GEO_BASIC,
	GEO_SPEED,
	GEO_TANKY,
	GEO_RESIST,


	NUM_GEOMETRY,
};

class RenderManager
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,

		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,

		U_TOTAL,
	};

	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
		TOTAL_RENDER_PASS
	};

	~RenderManager();
	list<Renderable*> renderableList;

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_gPassShaderID;
	unsigned m_parameters[U_TOTAL];

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Camera* camera;

	Light lights[1];
	RENDER_PASS m_renderPass;
	DepthFBO m_lightDepthFBO;

	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	void Init();
	void InitMesh();

	static RenderManager* GetInstance();
	void RenderObj(Renderable* obj);
	void RenderMesh(GEOMETRY_TYPE meshID,bool lightEnabled, bool fog);
	void RenderMesh(GEOMETRY_TYPE meshID, Vector3 pos, Vector3 scale, Vector3 rotation, bool lightEnabled, bool fog);
	void RenderTextOnScreen(string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(GEOMETRY_TYPE geo, bool lightEnabled, float size, float x, float y);
	void RenderGPass(int sceneID = 0);
	void RenderMain(int sceneID = 0);
	void Update(double dt);
	void AddRenderable(Renderable* entity);
	void RemoveRenderable(Renderable* entity);
	void SetCamera(Camera* cam);
	void UpdateBillboard(int sceneID = 0);

private:
	
	static RenderManager* instance;
	
protected:
	RenderManager();

};

#endif