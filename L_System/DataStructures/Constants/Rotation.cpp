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

        Rotation::Rotation(float angle, glm::vec3 rotation)
                    :
                        angle{angle},
                        rotation{rotation},
                        enabled{true}
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

        bool Rotation::isEnabled() const
        {
            return enabled;
        }
    }
}