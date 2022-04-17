#pragma once

#include "Mesh.h"
#include "Material.h"
#include <glm/glm.hpp>


class Object
{
public:
	Object();
	Object(Mesh mesh, glm::mat4 modelMatrix, Material mat);
	Material getMat();
	glm::mat4 getModelMatrix();
	void setModelMatrix(glm::mat4 _modelMatrix);
	Mesh getMesh();
	~Object();

private:
	Mesh mesh;
	glm::mat4 modelMatrix;
	Material mat;
};

