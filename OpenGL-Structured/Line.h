#pragma once

#include "Drawable.h"
#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


class Line: public Drawable
{
public:
	Line();
	void initVBO();
	GLuint getglVertexAtrrib();
	
	GLuint getVAO();
	void draw();
	void addVertex(glm::vec4 vertex);
	~Line();

private:
	std::vector<glm::vec4> vertices;
	GLuint glVertexAtrrib, glIndices, VAO;

};


