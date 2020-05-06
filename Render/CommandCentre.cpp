//
// Created by binybrion on 4/30/20.
//

#include <ModelLoading/Model.h>
#include <ext/matrix_transform.hpp>
#include "CommandCentre.h"

namespace Render
{
    void CommandCentre::addModel(const ::ModelLoading::Model &model)
    {
        modelVao.addModel(model);

        glm::mat4x4 transform = glm::mat4x4{1.0};


        modelVao.addModelInstances(model.getModelFileName(), 3, {transform});
    }

    void CommandCentre::checkRayIntersection(int screenWidth, int screenHeight, int mouseX, int mouseY)
    {
        glm::vec3 worldCoordinates = cameraObject.getWorldCoordinates(screenWidth, screenHeight, mouseX, mouseY);

        glm::vec3 rayDirection = worldCoordinates - cameraObject.getPosition();

        modelVao.checkRayIntersection(cameraObject.getPosition(), rayDirection);
    }

    Camera::CameraObject &CommandCentre::getCamera()
    {
        return cameraObject;
    }

    void CommandCentre::initialize()
    {printf("initialize model vao \n");
        float backgroundColourComponents = 0.15f;

        backgroundColour = glm::vec3{backgroundColourComponents, backgroundColourComponents, backgroundColourComponents};

        shaderManager.addShaderProgram("GridSystem",
                                    "/home/binybrion/CLionProjects/Voxel_L_System/Render/Shaders/GridSystem/vertexShader.txt",
                                    "/home/binybrion/CLionProjects/Voxel_L_System/Render/Shaders/GridSystem/fragmentShader.txt");

        shaderManager.addShaderProgram("UserModels",
                                       "/home/binybrion/CLionProjects/Voxel_L_System/Render/Shaders/UserModels/vertexShader.txt",
                                       "/home/binybrion/CLionProjects/Voxel_L_System/Render/Shaders/UserModels/fragmentShader.txt");

        axisVao.initialize();
        gridVao.initialize();
        modelVao.initialize();
    }

    void CommandCentre::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, 1.0f);

        shaderManager.getShaderProgram("GridSystem").bind();

        shaderManager.getShaderProgram("GridSystem").setUniformValue("viewMatrix", convertQMatrix(cameraObject.getViewMatrix()));
        shaderManager.getShaderProgram("GridSystem").setUniformValue("projectionMatrix", convertQMatrix(cameraObject.getProjectionMatrix()));

        axisVao.render();
        gridVao.render();

        shaderManager.getShaderProgram("UserModels").bind();

        shaderManager.getShaderProgram("UserModels").setUniformValue("viewMatrix", convertQMatrix(cameraObject.getViewMatrix()));
        shaderManager.getShaderProgram("UserModels").setUniformValue("projectionMatrix", convertQMatrix(cameraObject.getProjectionMatrix()));

        modelVao.render();
    }

    void CommandCentre::resetIntersectionColours()
    {
        modelVao.resetIntersectionColours();
    }

    QMatrix4x4 CommandCentre::convertQMatrix(const glm::mat4 &matrix) const
    {
        QMatrix4x4 matrix4X4;

        matrix4X4.setColumn(0, QVector4D{matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]});
        matrix4X4.setColumn(1, QVector4D{matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]});
        matrix4X4.setColumn(2, QVector4D{matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]});
        matrix4X4.setColumn(3, QVector4D{matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]});

        return matrix4X4;
    }

    QVector3D CommandCentre::convertQVector(const glm::vec3 &vector) const
    {
        QVector3D vector3D;

        vector3D.setX(vector.x);
        vector3D.setY(vector.y);
        vector3D.setZ(vector.z);

        return vector3D;
    }
}
