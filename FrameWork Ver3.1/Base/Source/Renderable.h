#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Entity.h"
#include "RenderManager.h"

class Renderable : public virtual Entity
{
public:
	Renderable();
	virtual ~Renderable();

	GEOMETRY_TYPE meshID;
	bool b_Render;
	bool b_lightEnabled;
	bool b_billboarded;
	bool b_3DbillBoard;
	bool b_shadows;
	bool b_fog;

	void UpdateBillboard();
	void SetParent(Renderable* parent);
	Renderable* GetParent();
private:
	Renderable* parent;
protected:


};
#endif