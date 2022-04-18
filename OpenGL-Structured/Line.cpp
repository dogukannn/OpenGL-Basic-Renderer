#include "Line.h"

Line::Line()
{
}

Line::~Line()
{
}

void Line::addVertex(glm::vec4 vertex)
{
	vertices.push_back(vertex);
}

void Line::initVBO()
{
	glGenVertexArrays(1, &VAO);
	//assert(vao > 0);
	glBindVertexArray(VAO);

	//glEnableVertexAttribArray(0);
	
	

	glGenBuffers(1, &glVertexAtrrib);
	

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, glVertexAtrrib);
	

	int vertices_size = vertices.size() * 4 * sizeof(float);

	glBufferData(GL_SHADER_STORAGE_BUFFER, vertices_size, &vertices[0], GL_STATIC_DRAW);
	

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
}

void Line::draw()
{
	glBindVertexArray(VAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glVertexAtrrib);
	glDrawArrays(GL_TRIANGLES, 0, 6 * (vertices.size() - 3));
}


GLuint Line::getglVertexAtrrib()
{
	return glVertexAtrrib;
}

GLuint Line::getVAO()
{
	return VAO;
}