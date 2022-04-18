#pragma once


#include "Material.h"
#include "StructuredMesh.h"
#include <glm/glm.hpp>


class Object
{
public:
	Object();
	Object(Drawable* mesh, glm::mat4 modelMatrix, Material mat);
	Material getMat();
	glm::mat4 getModelMatrix();
	void setModelMatrix(glm::mat4 _modelMatrix);
	Drawable* getMesh();
	~Object();

private:
	Drawable* mesh;
	glm::mat4 modelMatrix;
	Material mat;
};

