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


typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;

static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

RenderManager renderManager;


static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

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

    glm::mat4 matr = glm::translate(glm::mat4(1.0), glm::vec3(centerX, centerY, centerZ));
    //glm::mat4 matr = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
    matr = glm::scale(matr, glm::vec3(1, 1, 1));
    matr = glm::rotate(matr, -90.0f, glm::vec3(1, 0, 0));
    matr = glm::translate(matr, glm::vec3(-centerX, -centerY, -centerZ));
    sceneManager.addObject(mesh, matr, mat);

    glm::vec4 point = glm::vec4(centerX, centerY, centerZ, 1);
    glm::vec4 minP = glm::vec4(minBound, 1);
    glm::vec4 maxP = glm::vec4(maxBound, 1);
    point = matr * point;
    minP = matr * minP;
    maxP = matr * maxP;
    zlen = sqrt((maxP.z - minP.z) * (maxP.z - minP.z));
    
    std::cout <<" sd " << point.w << std::endl;

    float fovyRad = (float)(45.0 / 180.0) * M_PI;
    glm::mat4 projectionMatrix = glm::perspective(fovyRad, 1.0f, 1.0f, zlen + 50);
    //projectionMatrix = glm::frustum(minBound.x, maxBound.x, minBound.y, maxBound.y, zlen, zlen*4);
    projectionMatrix = glm::frustum(minP.x, maxP.x, minP.y, maxP.y, zlen, zlen * 3);

    //glm::vec3 camEye(centerX, centerY, centerZ + zlen * 1.5f);
    glm::vec3 camEye(point.x, point.y, point.z + zlen * 2.0f);
    //glm::vec3 camEye(0, 0, 0 + zlen * 1.5f);
    glm::vec3 camPos = camEye + glm::vec3(0, 0, -1);
    glm::vec3 camUp(0, 1, 0);

    glm::mat4 viewingMatrix = glm::lookAt(camEye, camPos, camUp);

    renderManager.setProjectionMatrix(projectionMatrix);
    renderManager.setViewingMatrix(viewingMatrix);

    //eyePos = camEye;


    std::cout << "before main loop" << std::endl;
    mainLoop(window, sceneManager);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
