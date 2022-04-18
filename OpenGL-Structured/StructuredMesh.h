#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Drawable.h"

struct Vertex
{
	int idx; 
	std::vector< int > vertList; //adj vvertices;
	std::vector< int > triList;
	std::vector< int > edgeList;

	Vertex(int i) : idx(i) {};
};

struct Edge
{
	int idx; //edges[idx]
	int v1i, v2i; //endpnts
	float length;
	Edge(int id, int v1, int v2) : idx(id), v1i(v1), v2i(v2) { computeLength(); };

	void computeLength()
	{
		length = 7;
	}
};


struct Triangle
{
	int idx; //tris[idx]
	int v1i, v2i, v3i;
	glm::vec3 center;
	glm::vec3 color;
	float area;
	Triangle(int id, int v1, int v2, int v3) : idx(id), v1i(v1), v2i(v2), v3i(v3) {};
};


class StructuredMesh : public Drawable
{
public:
	

	StructuredMesh();
	StructuredMesh(std::string obb_name);
	void initVBO();
	GLuint getglVertexAtrrib();
	GLuint getglIndices();
	GLuint getVAO();
	void draw();

	void addTriangle(int v1, int v2, int v3);
	void addEdge(int v1, int v2);
	void addVertex();
	bool makeVertsNeighbor(int v1i, int v2i);


	~StructuredMesh();
	glm::vec3 minBound, maxBound;

private:
	float* vertices;
	float* normals;
	float* colors;
	int* indices;
	int vertices_size, indices_size;
	

	GLuint glVertexAtrrib, glIndices, VAO;
	

	std::vector< Vertex* > verts;
	std::vector< Triangle* > tris;
	std::vector< Edge* > edges;
};




/*#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh();
	Mesh(std::string obb_name);
	virtual void initVBO();
	virtual GLuint getglVertexAtrrib();
	virtual GLuint getglIndices();
	virtual GLuint getVAO();
	virtual void draw();

	~Mesh();

	float minx, maxx;
	float miny, maxy;
	float minz, maxz;

private:
	float* vertices;
	float* normals;
	int vertices_size, indices_size;
	int* indices;

	GLuint glVertexAtrrib, glIndices, VAO;

};*/

