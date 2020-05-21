//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_AXISVAO_H
#define VOXEL_L_SYSTEM_AXISVAO_H

#include "Render/VBO/VBOWapper.h"
#include <vec3.hpp>

namespace Render
{
    namespace GridSystem
    {
        /**
         *  Handles storing the required data for the axis in the scene as well as rendering those axis.
         */

        class AxisVAO : public QOpenGLFunctions_4_2_Core
        {
            public:

                /**
                 * Stores the data required for rendering in the appropriate buffers, and the required layouts are
                 * specified for rendering.
                 */
                void initialize();

                /**
                 * Renders the axis in the scene.
                 */
                void render();

            private:
                unsigned int vao;
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::vec3> axisVBO;
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::vec3> axisColourVBO;
                unsigned int numberLinePoints = 0;
        };
    }
}

#endif //VOXEL_L_SYSTEM_AXISVAO_H
