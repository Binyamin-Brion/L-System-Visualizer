//
// Created by binybrion on 5/7/20.
//

#ifndef VOXEL_L_SYSTEM_MODELINSTANCEPLACEMENT_H
#define VOXEL_L_SYSTEM_MODELINSTANCEPLACEMENT_H

#include <QtCore/QString>
#include <mat4x4.hpp>

namespace L_System
{
    namespace Interpretation
    {
        /**
         * Holds the information to render an instance of a model based off of the transformation specified in a L-Script.
         */

        struct ModelInstancePlacement
        {
            QString variableName;
            QString modelName;
            glm::mat4x4 transformation;
        };
    }
}

#endif //VOXEL_L_SYSTEM_MODELINSTANCEPLACEMENT_H
