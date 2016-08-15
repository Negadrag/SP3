#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::Terrain(GEOMETRY_TYPE meshID,const std::string &filePath, Vector3 scale)
{
	this->meshID = meshID;
	LoadHeightMap(filePath.c_str(), this->heightMap);
	this->scale = scale;
	this->b_shadows = true;
}

Terrain::~Terrain()
{
}

float Terrain::ReadHeightMap(float x, float z)
{
	if (x < -0.5f*scale.x || x > 0.5f*scale.x || z < -0.5f*scale.z || z > 0.5f*scale.z)
		return 0;
	if (heightMap.size() == 0)
		return 0;

	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

	unsigned zCoord = (unsigned)(( (z/scale.z) + 0.5f) * terrainSize);
	unsigned xCoord = (unsigned)(( (x/scale.x) + 0.5f) * terrainSize);

	return (float)(heightMap[zCoord * terrainSize + xCoord] / 256.f) * scale.y;
}

void Terrain::Init(GEOMETRY_TYPE meshID, const std::string &filePath, Vector3 scale)
{
	this->meshID = meshID;
	LoadHeightMap(filePath.c_str(), this->heightMap);
	this->scale = scale;
}