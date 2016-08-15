#ifndef TERRAIN_H
#define TERRAIN_H

#include "LoadHmap.h"
#include "Renderable.h"

class Terrain: public Renderable
{
public:
	Terrain();
	Terrain(GEOMETRY_TYPE meshID,const std::string &filePath,Vector3 scale);
	~Terrain();

	float ReadHeightMap(float x, float z);
	void Init(GEOMETRY_TYPE meshID, const std::string &filePath, Vector3 scale);

private:

protected:
	std::vector<unsigned char> heightMap;

};

#endif