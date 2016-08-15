#include "Renderable.h"


Renderable::Renderable() :Entity()
{
	meshID = GEO_NULL;
	b_Render = true;
	b_lightEnabled = false;
	b_billboarded = false;
	b_3DbillBoard = false;
	b_shadows = false;
	b_fog = false;
	this->parent = nullptr;
	RenderManager::GetInstance()->AddRenderable(this);
}

Renderable::~Renderable()
{
	RenderManager::GetInstance()->RemoveRenderable(this);
}

void Renderable::UpdateBillboard()
{
	if (b_billboarded == true)
	{
		Vector3 distance = RenderManager::GetInstance()->camera->position - this->pos;
		this->rotation.y = Math::RadianToDegree(atan2(distance.x, distance.z));
	}
}


void Renderable::SetParent(Renderable* parent)
{
	this->parent = parent;
}

Renderable* Renderable::GetParent()
{
	return this->parent;
}