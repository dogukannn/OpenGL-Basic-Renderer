#pragma once
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

};

