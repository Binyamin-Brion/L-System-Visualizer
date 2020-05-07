//
// Created by binybrion on 5/6/20.
//

#include "Rotation.h"

namespace L_System
{
    namespace DataStructures
    {
        Rotation::Rotation()
                    :
                        enabled{false}
        {

        }

        Rotation::Rotation(StackOperation stackOperation, float angle, glm::vec3 rotation)
                    :
                        stackOperation{stackOperation},
                        angle{angle},
                        rotation{rotation}
        {

        }

        float Rotation::getAngle() const
        {
            return angle;
        }

        glm::vec3 Rotation::getRotation() const
        {
            return rotation;
        }

        StackOperation Rotation::getStackOperation() const
        {
            return stackOperation;
        }

        bool Rotation::isEnabled() const
        {
            return enabled;
        }
    }
}