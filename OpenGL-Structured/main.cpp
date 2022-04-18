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
#include "StructuredMesh.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Object.h"
#include "Material.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>


RenderManager renderManager;
bool isDragging = false;
bool isMoving = false;
glm::vec3 currentDirVector(0, 0, 0);
glm::vec3 recentPosVector(0, 0, 0);

#define LOGVEC3(name, a) std::cout<<name << " : " << a.x << ", " << a.y << ", " << a.z <<std::endl


void focusCameraToBoundingSphere(RenderManager& renderManager, glm::vec3 center, float radius)
{
    glm::mat4 projectionMatrix = glm::frustum(center.x - radius * 1, center.x + radius * 1, center.y - radius * 1, center.y + radius * 1, radius, radius * 8);    
    
    glm::vec3 camEye(center.x, center.y, center.z + radius * 2.5f);
    glm::vec3 camPos = camEye + glm::vec3(0, 0, -1);
    glm::vec3 camUp(0, 1, 0);

    glm::mat4 viewingMatrix = glm::lookAt(camEye, camPos, camUp);

    std::cout << " cam loc x: " << camEye.x << " y: " << camEye.y << " z: " << camEye.z << std::endl;
    renderManager.setProjectionMatrix(projectionMatrix);
    renderManager.setViewingMatrix(viewingMatrix);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        recentPosVector = glm::vec3(xpos, ypos, 0);
        std::cout << "mouse clicked" << "(x,y) : " << xpos << ", " << ypos << std::endl;
        isDragging = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        std::cout << "mouse released" << "(x,y) : " << xpos << ", " << ypos << std::endl;
        isDragging = false;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (isDragging)
    {
        isMoving = true;
        glm::vec3 posVector(xpos, ypos, 0);

        currentDirVector = posVector - recentPosVector;

        if (glm::length(currentDirVector) != 0)
            currentDirVector = glm::normalize(currentDirVector);

        //LOGVEC3("current_dir", currentDirVector);

        recentPosVector = posVector;
        //std::cout << "mouse clicked" << "(x,y) : " << xpos << ", " << ypos << std::endl;

    }
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
        float rotRad = (float)(1.0f / 180.f) * M_PI;
        glm::vec3 perp(currentDirVector.y, currentDirVector.x, 0);

        glm::quat qrot(cos(rotRad / 2), perp.x * sin(rotRad / 2), perp.y * sin(rotRad / 2), perp.z * sin(rotRad / 2));

        Object* object = scene.getObjectAt(0);
        glm::mat4 model_matrix = object->getModelMatrix();

        if (isMoving && glm::length(perp) != 0)
        {
            //LOGVEC3("perp ", perp);
            
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(0, 0, -5));
            model = model * glm::toMat4(qrot) ;
            //model = glm::rotate(model, rotRad, perp);
            model = glm::translate(model, glm::vec3(0, 0, 5));
            model_matrix = model * model_matrix;
            //std::cout << glm::to_string(model_matrix) << std::endl;
        }
            
        isMoving = false;
        object->setModelMatrix(model_matrix);

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwMakeContextCurrent(window);
    //gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    gladLoadGL();
    glfwSwapInterval(1);

    
    StructuredMesh mesh = StructuredMesh("0.off");
    mesh.initVBO();
    Material mat;
    mat.initShaders("vert.glsl", "frag.glsl");
    SceneManager sceneManager;
    
    
    renderManager.reshape(window, 640, 480);
    renderManager.start();


    glm::vec3 minBound = mesh.minBound;
    glm::vec3 maxBound = mesh.maxBound;


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

    /*glm::mat4 matr = glm::translate(glm::mat4(1.0), glm::vec3(centerX, centerY, centerZ));
    matr = glm::scale(matr, glm::vec3(1, 1, 1));
    matr = glm::rotate(matr, -90.0f, glm::vec3(1, 0, 0));
    matr = glm::translate(matr, glm::vec3(-centerX, -centerY, -centerZ));
    sceneManager.addObject(mesh, matr, mat);*/

    //glm::mat4 matr = glm::translate(glm::mat4(1.0), glm::vec3(centerX, centerY, centerZ));
    
    glm::mat4 matr = glm::mat4(1);
    matr = glm::translate(matr, glm::vec3(0, 0, -5));
    matr = glm::rotate(matr, -90.0f, glm::vec3(1, 0, 0));
    matr = glm::scale(matr, glm::vec3(0.05f, 0.05f, 0.05f));
    matr = glm::translate(matr, glm::vec3(-centerX, -centerY, -centerZ));
    sceneManager.addObject(mesh, matr, mat);

    
    
    
   
   
    glm::vec3 center(centerX, centerY, centerZ);

    //focusCameraToBoundingSphere(renderManager, center, radius);
   
   

    //eyePos = camEye;

    glEnable(GL_DEPTH_TEST);
    std::cout << "before main loop" << std::endl;
    mainLoop(window, sceneManager);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
