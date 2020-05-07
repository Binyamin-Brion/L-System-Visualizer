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
        /**
         * Represents a rotation transformation that changes the rotation of a L-Script.
         */

        class Rotation
        {
            public:

                /**
                 *  Default constructed rotation that does not affect the script's rotation.
                 *
                 *  Should only be used when defining other required elements of a script's elements, such as
                 *  a script Constant.
                 */
                Rotation();

                /**
                 * Creates a rotation that affects the script's rotation transformation.
                 *
                 * @param stackOperation how this rotation should affect the rotation stack of the script
                 * @param angle the angle in degrees of the rotation
                 * @param rotation the axis along which the rotation should occur
                 */
                Rotation(StackOperation stackOperation, float angle, glm::vec3 rotation);

                /**
                 * Get the angle of the rotation.
                 *
                 * @return rotation's angle
                 */
                float getAngle() const;

                /**
                 * Get the axis along which the rotation occurs
                 *
                 * @return rotation's axis
                 */
                glm::vec3 getRotation() const;

                /**
                 * Get the stack operation of the rotation.
                 *
                 * @return rotation's stack operation
                 */
                StackOperation getStackOperation() const;

                /**
                 * Get whether the Rotation is active, as in it affects the script's rotation transformation.
                 *
                 * If the Rotation is default constructed, the returned value is false; otherwise true.
                 *
                 * @return true if the Rotation modifies the script's rotation transformation
                 */
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
