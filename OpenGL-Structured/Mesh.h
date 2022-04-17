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
	void initVBO();
	GLuint getglVertexAtrrib();
	GLuint getglIndices();
	GLuint getVAO();
	void draw();
	
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

