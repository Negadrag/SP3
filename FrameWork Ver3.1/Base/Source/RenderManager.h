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
	GEO_PATH,
	GEO_GRASS_DARKGREEN,
	GEO_GRASS,
	GEO_OBJECT,
	GEO_TEXT,
	GEO_TERRAIN,
	GEO_ADJUSTBAR,

	GEO_SKYTOP,
	GEO_SKYBOTTOM,
	GEO_SKYFRONT,
	GEO_SKYBACK,
	GEO_SKYLEFT,
	GEO_SKYRIGHT,

	GEO_BLUECUBE,
	GEO_REDCUBE,
	GEO_YELLOWCUBE,
	GEO_GREENCUBE,

	GEO_DUNGEONWALL,
	GEO_MENUTOWN,
	GEO_HUD_BG,

	GEO_CUBE2,
	GEO_WATER,

	//Particles
	GEO_SMOKEPARTICLES,
	GEO_PARTICLE_WHITE,

	GEO_PARTICLE_BLUE,
	GEO_PARTICLE_RED,
	GEO_PARTICLE_YELLOW,
	GEO_PARTICLE_GREEN,

	//TSL
	GEO_SKYPLANE,
	GEO_LIGHT_DEPTH_QUAD,

	//Tower
	GEO_ARROWTOWER,
	GEO_ARROW,

	GEO_CANNONTOWER,
	GEO_CANNON,
	GEO_CANNONBLAST,

	GEO_CAPTURETOWER,
	GEO_CAPTUREBASE,
	GEO_CAPTURERING1,
	GEO_CAPTURERING2,
	GEO_CAPTURERING3,
	GEO_CAPTUREORB,

	GEO_BUFFTOWER,

	GEO_ICETOWER,
	GEO_ICEBASE,
	GEO_ICESHOT,
	GEO_ICEPARTICLE,

	GEO_POISONTOWER,
	GEO_POISONBASE,
	GEO_POISONARROW,

	GEO_SPEEDMODEL,
	GEO_SPEEDTOWER,
	GEO_BULLET,

	GEO_MORTARTOWER,
	GEO_MORTARBASE,
	GEO_MORTARCANNON,
	GEO_MORTARCANNONBALL,

	GEO_LIVES,
	GEO_COIN,
	GEO_ICEESSENCE,
	GEO_SPEEDESSENCE,
	GEO_TANKESSENCE,
	GEO_POISONESSENCE,

	GEO_ARROWTOWER_FROST,
	GEO_CANNONTOWER_FROST,
	GEO_SPEEDTOWER_FROST,
	GEO_POISONTOWER_FROST,
	GEO_ICETOWER_FROST,
	GEO_MORTARTOWER_FROST,
	GEO_MORTARCANNON_FROST,

	//Monster
	GEO_BASIC,
	GEO_SPEED,
	GEO_TANKY,
	GEO_ICE,
	GEO_BOSS,

	GEO_BASIC_FROST,
	GEO_SPEED_FROST,
	GEO_TANKY_FROST,
	GEO_BOSS_FROST,

	GEO_SPEED_POISON,
	GEO_TANKY_POISON,
	GEO_ICE_POISON,
	GEO_BOSS_POISON,

	GEO_SAVE,

	GEO_FOR_VALOR,
	GEO_ESSENCE,

	GEO_HP,
	GEO_HP2,

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
	vector<list<Renderable*>> renderableList;

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
	void RenderText(std::string text, Color color, Vector3 pos, Vector3 scale, Vector3 rotation, bool enableLight = false, bool fog = false);
	void RenderTextOnScreen(string text, Color color, float size, float x, float y, float z = 0.f);
	void RenderMeshOnScreen(GEOMETRY_TYPE geo, bool lightEnabled, float size, float x, float y);
	void RenderMeshOnScreen(GEOMETRY_TYPE geo, bool lightEnabled, Vector3 position, Vector3 scale, Vector3 rotation);
	void RenderGPass(int sceneID = 0);
	void RenderMain(int sceneID = 0);
	void Update(double dt);
	void AddRenderable(Renderable* entity);
	void RemoveRenderable(Renderable* entity);
	void SetCamera(Camera* cam);
	void UpdateBillboard(int sceneID = 0);
	void SetLight(Vector3 pos);

private:
	
	static RenderManager* instance;
	
protected:
	RenderManager();

};

#endif