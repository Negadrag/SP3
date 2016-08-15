#ifndef MESH_H
#define MESH_H

#include <string>
#include "Material.h"


class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	virtual ~Mesh();
	virtual void Render();
	virtual void Render(unsigned offset, unsigned count);

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;
	unsigned textureID;

	//for multi texturing
	static const int MAX_TEXTURES = 2;
	unsigned textureArray[MAX_TEXTURES];

};

#endif