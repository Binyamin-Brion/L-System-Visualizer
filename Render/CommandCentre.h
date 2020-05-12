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
             * Checks for an intersection with the users cursor with an instance of model.
             *
             * @param screenWidth width of the OpenGL widget
             * @param screenHeight height of the OpenGL widget
             * @param mouseX x-position of the cursor relative to the OpenGL widget
             * @param mouseY y-position of the cursor relative to the OpenGL widget
             */
            void checkRayIntersection(int screenWidth, int screenHeight, int mouseX, int mouseY);

            /**
             * Removes all rendering data. Afterwards, nothing will be rendered.
             */
            void clearData();

            /**
             * Get the reference to the camera used for rending.
             *
             * @return reference to the camera
             */
            Camera::CameraObject& getCamera();

            /**
             * Initializes are OpenGL data structures required for rending.
             */
            void initialize();

            /**
             * Renders the scene with the grid system and the mesh(es).
             */
            void render();

            /**
             * Resets the the intersection colours for all instances of all models.
             */
            void resetIntersectionColours();

        private:

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
