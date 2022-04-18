#pragma once


#include "Material.h"
#include "StructuredMesh.h"
#include <glm/glm.hpp>


class Object
{
public:
	Object();
	Object(StructuredMesh mesh, glm::mat4 modelMatrix, Material mat);
	Material getMat();
	glm::mat4 getModelMatrix();
	void setModelMatrix(glm::mat4 _modelMatrix);
	StructuredMesh getMesh();
	~Object();

private:
	StructuredMesh mesh;
	glm::mat4 modelMatrix;
	Material mat;
};

