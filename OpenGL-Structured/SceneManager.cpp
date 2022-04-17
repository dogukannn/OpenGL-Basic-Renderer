#include "SceneManager.h"


SceneManager::SceneManager()
{


}

void SceneManager::addObject(Mesh mesh, glm::mat4 matrix, Material mat)
{
	objects.push_back(Object(mesh, matrix, mat));
}

Object SceneManager::getObjectAt(int index)
{
	return objects[index];
}

/*glm::mat4 SceneManager::getViewingMatrix()
{
	return viewingMatrix;
}

glm::mat4 SceneManager::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::vec3 SceneManager::getEyePos()
{
	return eyePos;
}*/


int SceneManager::getObjectCount()
{
	return objects.size();
}



SceneManager::~SceneManager()
{
}