//
// Created by binybrion on 5/5/20.
//

#ifndef VOXEL_L_SYSTEM_TRANSLATION_H
#define VOXEL_L_SYSTEM_TRANSLATION_H

#include <vec3.hpp>
#include "StackOperation.h"

namespace L_System
{
    namespace DataStructures
    {
        struct Translation
        {
            bool enabled;
            StackOperation stackOperation;
            glm::vec3 translation;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TRANSLATION_H
