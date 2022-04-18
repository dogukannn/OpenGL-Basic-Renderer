#pragma once
#include <glad/glad.h>
#include <vector>
#include "SceneManager.h"
#include "StructuredMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GLFW/glfw3.h>

class RenderManager
{
public:
	RenderManager();
	void start();
	void reshape(GLFWwindow* window, int w, int h);
	void setViewingMatrix(glm::mat4 _viewingMatrix);
	void setProjectionMatrix(glm::mat4 _projectionMatrix);
	
	void render(SceneManager scene);
	~RenderManager();

private:
	GLuint gWidth, gHeight;
	std::vector<GLuint> glProgram;
	glm::mat4 viewingMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 eyePos;
};

