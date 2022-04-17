#include "Object.h"


Object::Object()
{
}

Object::Object(Mesh _mesh, glm::mat4 _modelMatrix, Material _mat)
{
	mesh = _mesh;
	modelMatrix = _modelMatrix;
	mat = _mat;
}

void Object::setModelMatrix(glm::mat4 _modelMatrix)
{
	modelMatrix = _modelMatrix;
}

Material Object::getMat()
{
	return mat;
}

glm::mat4 Object::getModelMatrix()
{
	return modelMatrix;
}

Mesh Object::getMesh()
{
	return mesh;
}

Object::~Object()
{
}