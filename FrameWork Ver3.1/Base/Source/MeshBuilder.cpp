#include <vector>
using namespace std;

#include <GL\glew.h>
#include "Vertex.h"
#include "MyMath.h"
#include "LoadOBJ.h"
#include "LoadHmap.h"

#include <iostream>
#include "MeshBuilder.h"
/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a crosshair;
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCrossHair(const std::string &meshName, float colour_r, float colour_g, float colour_b, float length)
{
	// Declare the variable to store a vertex and the buffer for storing vertices
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	// Vertex #1
	v.pos.Set(-length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #2
	v.pos.Set(length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #3
	v.pos.Set(0, -length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #4
	v.pos.Set(0, length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	v.pos.Set(-0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 1.0f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 1.0f);
	vertex_buffer_data.push_back(v);
	
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	
	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 36;
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR, float innerR)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;

	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * degreePerSlice;
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(2 * slice + 0);
		index_buffer_data.push_back(2 * slice + 1);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 4);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 2);
	}

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;

	for(unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			Vertex v;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack)
	{
		for(unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	Vertex v;
	float degreePerSlice = 360.f / numSlice;

	for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
	{
		float theta = slice * degreePerSlice;
		Vector3 normal(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		normal.Normalize();

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
		
		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if(!success)
		return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh *mesh = new Mesh(meshName);
	
	mesh->mode = Mesh::DRAW_TRIANGLES;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for(unsigned i = 0; i < numRow; ++i)
	{
		for(unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);
	
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}


Mesh* MeshBuilder::GenerateSkyPlane(const std::string &meshName, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	//check number of slices(cannot go above 256)
	if (slices < 1)
	{
		slices = 1;
	}
	else if (slices>256)
	{
		slices = 256;
	}

	//calculate some values that are necessary for the plane size

	float planeSize = PlanetRadius * 2.f;
	float delta = planeSize / (float)slices;
	float texdelta = 2.f / (float)slices;

	float heightOffset = ((planeSize / 2.f) * (planeSize / 2.f)) / (0.5 * planeSize * 0.5 * planeSize) * AtmosphereRadius;
	//float heightOffset = AtmosphereRadius / 2.f;
	//calculate vertex
	for (int z = 0; z <= slices; ++z)
	{
		for (int x = 0; x <= slices; ++x)
		{
			float xDist = (-0.5f * planeSize) + ((float)x * delta);
			float zDist = (-0.5f * planeSize) + ((float)z * delta);

			float xHeight = ( (xDist * xDist) / (0.5 * planeSize * 0.5 * planeSize)) *  AtmosphereRadius;
			float zHeight =( (zDist * zDist) / (0.5 * planeSize * 0.5 * planeSize) ) * AtmosphereRadius;

			float height = xHeight + zHeight;

			Vertex tv;//temp vertex
			tv.pos.Set(xDist, heightOffset - height, zDist);

			//calculate the texture coordinates
			tv.texCoord.u = hTile * ((float)x * texdelta * 0.5f);
			tv.texCoord.v = vTile * (1.0f - (float)z * texdelta * 0.5f);

			tv.color = color;

			vertex_buffer_data.push_back(tv);

		}
	}
	// Calculate the indices
	int index = 0;

	for (int i = 0; i < slices; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			int startvert = (i * (slices + 1) + j);
			index_buffer_data.push_back(startvert);//triangle 1
			index_buffer_data.push_back(startvert + 1);
			index_buffer_data.push_back(startvert + slices + 1);

			index_buffer_data.push_back(startvert + 1); // triangle2
			index_buffer_data.push_back(startvert + slices + 2);
			index_buffer_data.push_back(startvert + slices + 1);

		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}

Vector3 CalculateTerrainNormal(float heightLeft, float heightRight, float heightUp, float heightDown)
{

	//top left as 0,0
	return Vector3( (heightLeft - heightRight), 2.f, (heightUp - heightDown) ).Normalize();
}

Mesh* MeshBuilder::GenerateTerain(const std::string &meshName, const std::string &filePath)
{
	std::vector<unsigned char> heightMap;
	if (LoadHeightMap(filePath.c_str(), heightMap))
	{
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		//256 is because we are saving the .raw as an 8bit greyscale image. Hence 8bit = 256.
		const float scaleFactor = 256.f;

		Vertex v; // temp vertex variable to store vertex info and pass into vertex buffer data
		unsigned int terrainSize = (unsigned int)sqrt((double)heightMap.size());
		for (unsigned int z = 0; z < terrainSize; ++z)
		{
			for (unsigned int x = 0; x < terrainSize; ++x)
			{
				float scaledHeight = (float)heightMap[z*terrainSize + x] / scaleFactor;
				v.pos.Set(static_cast<float>(x)/terrainSize - 0.5f,scaledHeight,static_cast<float>(z)/terrainSize-0.5f);

				v.color.Set(scaledHeight,scaledHeight,scaledHeight);

				v.texCoord.Set((float)x / terrainSize * 8, 1.f - (float)z / terrainSize * 8);//value 8 to repeat texture 8x across the plane

				vertex_buffer_data.push_back(v);
			}
		}

		for (unsigned int z = 1; z < terrainSize-1; ++z)
		{
			for (unsigned int x = 1; x < terrainSize-1; ++x)
			{
				//use the surrounding terrain height to get an estimated normal.
				vertex_buffer_data[z * terrainSize + x].normal = CalculateTerrainNormal(vertex_buffer_data[z * terrainSize + x - 1].pos.y,
																						vertex_buffer_data[z * terrainSize + x + 1].pos.y,
																						vertex_buffer_data[(z - 1) * terrainSize + x].pos.y,
																						vertex_buffer_data[(z + 1) * terrainSize + x].pos.y);
			}
		}

		for (unsigned int z = 0; z < terrainSize - 1; ++z)
		{
			for (unsigned int x = 0; x < terrainSize - 1; ++x)
			{
				//triangle 1
				index_buffer_data.push_back(terrainSize* z + x);
				index_buffer_data.push_back(terrainSize*(z + 1) + x);
				index_buffer_data.push_back(terrainSize* z + x + 1);

				//triangle 2
				index_buffer_data.push_back(terrainSize*(z + 1) + x + 1);
				index_buffer_data.push_back(terrainSize* z + x + 1);
				index_buffer_data.push_back(terrainSize*(z + 1) + x);
			}
		}

		Mesh *mesh = new Mesh(meshName);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

		mesh->indexSize = index_buffer_data.size();
		mesh->mode = Mesh::DRAW_TRIANGLES;

		return mesh;
	}

	return nullptr;
}
//
//Mesh* MeshBuilder::GenerateSpriteAnimation(const std::string &meshName, unsigned numRow, unsigned numCol)
//{
//	Vertex v;
//	std::vector<Vertex> vertex_buffer_data;
//	std::vector<GLuint> index_buffer_data;
//
//	float width = 1.f / numCol;
//	float height = 1.f / numRow;
//	int offset = 0;
//	for (unsigned i = 0; i < numRow; ++i)
//	{
//		for (unsigned j = 0; j < numCol; ++j)
//		{
//			float u1 = j * width;
//			float v1 = 1.f - height - i * height;
//			v.pos.Set(-0.5f, -0.5f, 0);
//			v.texCoord.Set(u1, v1);
//			vertex_buffer_data.push_back(v);
//
//			v.pos.Set(0.5f, -0.5f, 0);
//			v.texCoord.Set(u1 + width, v1);
//			vertex_buffer_data.push_back(v);
//
//			v.pos.Set(0.5f, 0.5f, 0);
//			v.texCoord.Set(u1 + width, v1 + height);
//			vertex_buffer_data.push_back(v);
//
//			v.pos.Set(-0.5f, 0.5f, 0);
//			v.texCoord.Set(u1, v1 + height);
//			vertex_buffer_data.push_back(v);
//
//			index_buffer_data.push_back(offset + 0);
//			index_buffer_data.push_back(offset + 1);
//			index_buffer_data.push_back(offset + 2);
//			index_buffer_data.push_back(offset + 0);
//			index_buffer_data.push_back(offset + 2);
//			index_buffer_data.push_back(offset + 3);
//			offset += 4;
//		}
//	}
//	Mesh *mesh = new Mesh(meshName);
//
//	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
//
//	mesh->indexSize = index_buffer_data.size();
//	mesh->mode = Mesh::DRAW_TRIANGLES;
//
//	return mesh;
//}

SpriteAnimation* MeshBuilder::GenerateSpriteAnimation(const string& meshName, unsigned int numRow, unsigned int numColumn) {

	if (numRow < 1) {
		numRow = 1;
	}
	if (numColumn < 1) {
		numColumn = 1;
	}

	vector<Vertex> vertex_buffer_data;
	vector<GLuint> index_buffer_data;

	float width = 1.f / numColumn;
	float height = 1.f / numRow;
	int offset = 0;

	Vertex v;
	v.normal.Set(0, 0, 1);
	for (unsigned i = 0; i < numRow; ++i) {
		for (unsigned j = 0; j < numColumn; ++j) {
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}

	SpriteAnimation* mesh = new SpriteAnimation(meshName, numRow, numColumn);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind the EBO

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}