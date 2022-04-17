#include "RenderManager.h"

RenderManager::RenderManager()
{
   
}

void RenderManager::start()
{
    float fovyRad = (float)(45.0 / 180.0) * M_PI;
    projectionMatrix = glm::perspective(fovyRad, 1.0f, 1.0f, 100.0f);

    glm::vec3 camEye(0, 0.0, 5.0);
    glm::vec3 camPos = camEye + glm::vec3(0, 0, -1);
    glm::vec3 camUp(0, 1, 0);

    viewingMatrix = glm::lookAt(camEye, camPos, camUp);

    eyePos = camEye;
}

void RenderManager::reshape(GLFWwindow* window, int w, int h)
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    gWidth = w;
    gHeight = h;

    glViewport(0, 0, w, h);

    /*float fovyRad = (float)(45.0 / 180.0) * M_PI;
    projectionMatrix = glm::perspective(fovyRad, 1.0f, 1.0f, 100.0f);

    glm::vec3 camEye(0, 0.0, 100.0);
    glm::vec3 camPos = camEye + glm::vec3(0, 0, -1);
    glm::vec3 camUp(0, 1, 0);

    viewingMatrix = glm::lookAt(camEye, camPos, camUp);

    eyePos = camEye;*/
}

void RenderManager::setViewingMatrix(glm::mat4 _viewingMatrix)
{
    viewingMatrix = _viewingMatrix;
}

void RenderManager::setProjectionMatrix(glm::mat4 _projectionMatrix)
{
    projectionMatrix = _projectionMatrix;
}

void RenderManager::render(SceneManager scene)
{
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    float timeValue = glfwGetTime();

    int object_size = scene.getObjectCount();

    for (int i = 0; i < object_size; i++)
    {
        Object object = *scene.getObjectAt(i);
        Material mat = object.getMat();
        
        glUseProgram(mat.glprogramIndex);
        glUniformMatrix4fv(mat.projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(mat.viewingMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewingMatrix));
        glUniformMatrix4fv(mat.modelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(object.getModelMatrix()));
        glUniform3fv(mat.eyePosLoc, 1, glm::value_ptr(eyePos));
        glUniform2f(mat.iResloc, gWidth, gHeight);
        glUniform1f(mat.iTimeloc, timeValue);

        //std::cout << "in render loop" << std::endl;

        object.getMesh().draw();
    }

}

RenderManager::~RenderManager()
{
}