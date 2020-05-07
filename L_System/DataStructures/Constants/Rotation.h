//
// Created by binybrion on 5/5/20.
//

#ifndef VOXEL_L_SYSTEM_ROTATION_H
#define VOXEL_L_SYSTEM_ROTATION_H

#include <vec3.hpp>
#include "StackOperation.h"

namespace L_System
{
    namespace DataStructures
    {
        struct Rotation
        {
            public:
                Rotation();
                Rotation(StackOperation stackOperation, float angle, glm::vec3 rotation);

                bool getAngle() const;
                glm::vec3 getRotation() const;
                StackOperation getStackOperation() const;

                bool isEnabled() const;

            private:

                bool enabled;
                StackOperation stackOperation;
                float angle;
                glm::vec3 rotation;
        };
    }
}

#endif //VOXEL_L_SYSTEM_ROTATION_H
