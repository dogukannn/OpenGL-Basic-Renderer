//========================================================================
// OpenGL triangle example
// Copyright (c) Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//! [code]

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "Mesh.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Object.h"
#include "Material.h"



RenderManager renderManager;


void focusCameraToBoundingSphere(RenderManager renderManager, glm::vec3 center, float radius)
{
    glm::mat4 projectionMatrix = glm::frustum(center.x - radius * 1, center.x + radius * 1, center.y - radius * 1, center.y + radius * 1, radius, radius * 4);    
    
    glm::vec3 camEye(center.x, center.y, center.z + radius * 2.5f);
    glm::vec3 camPos = camEye + glm::vec3(0, 0, -1);
    glm::vec3 camUp(0, 1, 0);

    glm::mat4 viewingMatrix = glm::lookAt(camEye, camPos, camUp);

    renderManager.setProjectionMatrix(projectionMatrix);
    renderManager.setViewingMatrix(viewingMatrix);
}


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
}

static void rehape_callback(GLFWwindow* window, int w, int h)
{
    renderManager.reshape(window, w, h);
}

void mainLoop(GLFWwindow* window, SceneManager scene)
{
    while (!glfwWindowShouldClose(window))
    {
        renderManager.render(scene);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, rehape_callback);


    glfwMakeContextCurrent(window);
    //gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    gladLoadGL();
    glfwSwapInterval(1);

    
    Mesh mesh("0.off");
    mesh.initVBO();
    Material mat;
    mat.initShaders("vert.glsl", "frag.glsl");
    SceneManager sceneManager;
    
    
    renderManager.reshape(window, 640, 480);
    renderManager.start();


    glm::vec3 minBound(mesh.minx, mesh.miny, mesh.minz);
    glm::vec3 maxBound(mesh.maxx, mesh.maxy, mesh.maxz);


    float centerZ = (maxBound.z + minBound.z) / 2;
    float centerX = (maxBound.x + minBound.x) / 2;
    float centerY = (maxBound.y + minBound.y) / 2;

    float xlen = sqrt((maxBound.x - minBound.x) * (maxBound.x - minBound.x));
    float ylen = sqrt((maxBound.y - minBound.y) * (maxBound.y - minBound.y));
    float zlen = sqrt((maxBound.z - minBound.z) * (maxBound.z - minBound.z));

    float radius;
    if (xlen > ylen && xlen > zlen) radius = xlen / 2;
    else if (ylen > xlen && ylen > zlen) radius = ylen / 2;
    else radius = zlen / 2;

    glm::mat4 matr = glm::translate(glm::mat4(1.0), glm::vec3(centerX, centerY, centerZ));
    matr = glm::scale(matr, glm::vec3(1, 1, 1));
    matr = glm::rotate(matr, -90.0f, glm::vec3(1, 0, 0));
    matr = glm::translate(matr, glm::vec3(-centerX, -centerY, -centerZ));
    sceneManager.addObject(mesh, matr, mat);

   
    glm::vec3 center(centerX, centerY, centerZ);

    focusCameraToBoundingSphere(renderManager, center, radius);
   
   

    //eyePos = camEye;


    std::cout << "before main loop" << std::endl;
    mainLoop(window, sceneManager);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
