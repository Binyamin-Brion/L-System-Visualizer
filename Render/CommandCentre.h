//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_COMMANDCENTRE_H
#define VOXEL_L_SYSTEM_COMMANDCENTRE_H

#include <Render/VAO/UserModels/ModelVAO.h>
#include "Render/VAO/GridSystem/GridVAO.h"
#include "VAO/GridSystem/AxisVAO.h"
#include "Shaders//ShaderManager.h"
#include "Camera/CameraObject.h"

namespace ModelLoading
{
    class Model;
}

namespace ProjectSaverLoader
{
    class ProjectDetails;
}

namespace Render
{
    /**
     *  The single place where all of the required OpenGL functionality is centralized in. All OpenGL classes are located
     *  here and the order at which they execute their functionality is controlled by this class.
     */

    class CommandCentre
    {
        public:

            /**
             * Uploads a model into GPU memory, allowing instances of the model to be rendered.
             *
             * Request is forwarded to the ModelVAO.
             *
             * @param model to be uploaded into vRam
             */
            void addModel(const ::ModelLoading::Model &model);

            /**
             *  Uploads instances of models that have been uploaded into vRam whose instances are specified in the Interpreter
             *  into vRam. At this point, those instances will be rendered in the next render loop.
            */
            void addModelInstances();

            /**
             * Adds an instance of the given model to the render. The instance will be centred around the origin.
             *
             * Any previous user action history is removed.
             *
             * This all is forwarded to the ModelVAO.
             *
             * @param modelFileName name of the instance to add
             */
            void addUserRequestedModelInstance(const QString &modelFileName);

            /**
             * Renders all of the model instances specified in the parameter. This is called when rerendering an interpretation result.
             *
             * Call is forwarded to the ModelVAO.
             *
             * @param modelInstances the list of instances for every model that needs instance(s) to be rendered
             */
            void addUserRequestedModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &modelInstances);

            /**
             * Checks for an intersection with the users cursor with an instance of model.
             *
             * @param screenWidth width of the OpenGL widget
             * @param screenHeight height of the OpenGL widget
             * @param mouseX x-position of the cursor relative to the OpenGL widget
             * @param mouseY y-position of the cursor relative to the OpenGL widget
             */
            void checkRayIntersection(int screenWidth, int screenHeight, int mouseX, int mouseY, bool appendIntersections);

            /**
             * Removes all (OpenGL) rendering data. Afterwards, nothing will be rendered.
             */
            void deleteOpenGLResources();

            /**
             * Deletes the selected instances. Afterwards they will no longer be rendered.
             *
             * This call is forwarded to ModelVAO.
             */
            void deleteSelectedInstances();

            /**
             * Forwards the request to export the currently rendered model instances to the ModelVAO.
             *
             * @param exportLocation location of the file to export to
             */
            void exportCurrentRender(const QString &exportLocation);

            /**
             * Get the reference to the camera used for rending.
             *
             * @return reference to the camera
             */
            Camera::CameraObject& getCamera();

            /**
             * Get the user defined models that are currently being rendered, whether selected or not.
             *
             * @return vector of rendered user added instances
             */
            std::vector<::ProjectSaverLoader::UserDefinedInstances> getUserDefinedInstances() const;

            /**
             * Initializes are OpenGL data structures required for rending.
             */
            void initialize();

            /**
             * Renders the scene with the grid system and the mesh(es).
             */
            void render();

            /**
             * Applies the given transformation to all of the rendered user added instances.
             *
             * This call is forwarded to the ModelVAO.
             *
             * @param transformationData transformation to apply to selected instances
             */
            void transformSelectedModels(const DataStructures::TransformationData &transformationData);

            /**
             * Undoes the most recent user action done in the render scene, if there is one.
             *
             * Request forwarded to ModelVAO.
             */
            void undoUserAction();

        private:

            /**
             * The previous render is removed- it is no longer rendered. The render data is still held in vRam however,
             * and may still be rendered later.
             */
            void clearPreviousRender();

            /**
             * Converts a glm matrix4x4 to the equivalent Qt Matrix4x4.
             *
             * @param matrix to be converted to the a Qt Matrix
             * @return equivalent Qt Matrix
             */
            QMatrix4x4 convertQMatrix(const glm::mat4 &matrix) const;

            /**
             * Converts a glm vector 3D to the equivalent Qt vector.
             *
             * @param vector to be converted to the a Qt vector
             * @return equivalent Qt vector
             */
            QVector3D convertQVector(const glm::vec3 &vector) const;

            /**
             * Gets the location of the shader folder containing shader files used for the render process.
             *
             * @return location to the shader folder
             */
            QString getShaderFolderLocation() const;

            Camera::CameraObject cameraObject;
            GridSystem::AxisVAO axisVao;
            GridSystem::GridVAO gridVao;
            VAO::ModelVAO modelVao;
            Shader::ShaderManager shaderManager;

            glm::vec3 backgroundColour;

            // Key: holds the model file name.
            // Value: holds the associated matrices for each depth result level.
            QHash<QString, std::vector<glm::mat4x4>> sortedInstancedMatrices;
    };
}

#endif //VOXEL_L_SYSTEM_COMMANDCENTRE_H
