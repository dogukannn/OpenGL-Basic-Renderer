#pragma once
#include <vector>
#include "Mesh.h"
#include "Object.h"
#include <glm/glm.hpp>

class SceneManager
{
public:
	SceneManager();
	//void addMesh(Mesh mesh);
	//void addMeshWithTransformation(Mesh mesh, glm::mat4 matrix);
	void addObject(Mesh mesh, glm::mat4 matrix, Material mat);
	Object* getObjectAt(int index);
	/*glm::mat4 getViewingMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getEyePos();*/

	int getObjectCount();
	~SceneManager();

private:
	std::vector<Object> objects;
	
};

