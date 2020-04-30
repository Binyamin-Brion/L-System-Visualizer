//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_COMMANDCENTRE_H
#define VOXEL_L_SYSTEM_COMMANDCENTRE_H

#include <Render/GridSystem/GridVAO.h>
#include "GridSystem/AxisVAO.h"
#include "Shaders//ShaderManager.h"
#include "Camera/CameraObject.h"

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

        private:

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
            Shader::ShaderManager shaderManager;

            glm::vec3 backgroundColour;
    };
}

#endif //VOXEL_L_SYSTEM_COMMANDCENTRE_H
