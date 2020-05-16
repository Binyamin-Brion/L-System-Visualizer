//
// Created by binybrion on 4/30/20.
//

#include <ModelLoading/Model.h>
#include <ext/matrix_transform.hpp>
#include "CommandCentre.h"
#include "../L_System/Interpretation/Interpreter.h"
#include <QHash>

namespace Render
{
    void CommandCentre::addModel(const ::ModelLoading::Model &model)
    {
        modelVao.addModel(model);
    }

    void CommandCentre::addModelInstances()
    {
        clearPreviousRender();

        /*
         * The instance matrices stored in the interpreter are stored linearly, meaning they are not grouped according to
         * the model. They are grouped in the order of the variables that appear left-to-right in the depth result.
         * However, when uploading a model to vRam, a vector of instance matrices is taken for each model. To prevent
         * potentially many uploads of different matrices for the same model (to vRam), the matrices are first sorted
         * according to the model and then uploaded in several calls (one for each depth level) for each model.
         */

        for(const auto &modelInstance : ::L_System::Interpretation::Interpreter::getResult().back())
        {
            // Ensure there is an entry for the model file name.
            auto entryLocation = sortedInstancedMatrices.find(modelInstance.modelName);

            if(!sortedInstancedMatrices.contains(modelInstance.modelName))
            {
                entryLocation = sortedInstancedMatrices.insert(modelInstance.modelName, std::vector<glm::mat4x4>{});
            }

            entryLocation.value().push_back(modelInstance.transformation);
        }

        // Iterate over the associated model matrices and uploaded them to the GPU memory.
        QHash<QString, std::vector<glm::mat4x4>>::const_iterator i = sortedInstancedMatrices.constBegin();

        while (i != sortedInstancedMatrices.constEnd())
        {
            modelVao.addModelInstances(i.key(), i.value());

            ++i;
        }
    }

    void CommandCentre::addUserRequestedModelInstance(const QString &modelFileName)
    {
        modelVao.addUserRequestedModelInstance(modelFileName);
    }

    void CommandCentre::addUserRequestedModelInstances(const std::vector< ::ProjectSaverLoader::UserDefinedInstances> &modelInstances)
    {
        modelVao.addUserRequestedModelInstances(modelInstances);
    }

    void CommandCentre::checkRayIntersection(int screenWidth, int screenHeight, int mouseX, int mouseY, bool appendIntersections)
    {
        glm::vec3 worldCoordinates = cameraObject.getWorldCoordinates(screenWidth, screenHeight, mouseX, mouseY);

        glm::vec3 rayDirection = worldCoordinates - cameraObject.getPosition();

        modelVao.checkRayIntersection(cameraObject.getPosition(), rayDirection, appendIntersections);
    }

    void CommandCentre::deleteOpenGLResources()
    {
        modelVao.deleteOpenGLResources();
    }

    void CommandCentre::deleteSelectedInstances()
    {
        modelVao.deleteSelectedInstances();
    }

    Camera::CameraObject &CommandCentre::getCamera()
    {
        return cameraObject;
    }

    std::vector<::ProjectSaverLoader::UserDefinedInstances> CommandCentre::getUserDefinedInstances() const
    {
        return modelVao.getUserDefinedInstances();
    }

    void CommandCentre::initialize()
    {
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

    void CommandCentre::transformSelectedModels(const DataStructures::TransformationData &transformationData)
    {
        modelVao.transformSelectedModels(transformationData);
    }

    void CommandCentre::undoUserAction()
    {
        modelVao.undoUserAction();
    }

    // Beginning of private functions

    void CommandCentre::clearPreviousRender()
    {
        // Find all of the models that have been loaded in the previous render, and ask the ModelVAO to remove all of the
        // instances associated with that model.

        QHash<QString, std::vector<glm::mat4x4>>::const_iterator i = sortedInstancedMatrices.constBegin();

        while (i != sortedInstancedMatrices.constEnd())
        {
            modelVao.removeModelInstances(i.key());

            ++i;
        }

        // Ensure that when starting a new render of an execution result, that any models from the previous render is
        // not included in the render. Not clearing this may result in models that have no instances (if the new execution
        // result does not use the same models), which isn't fatal, but does lead to redundant draw calls.
        sortedInstancedMatrices.clear();
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
