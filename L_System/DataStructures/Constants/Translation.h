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
        /**
         * Represents a translation transformation that changes the rotation of a L-Script.
         */

        class Translation
        {
            public:

                /**
                 *  Default constructed translation that does not affect the script's translation.
                 *
                 *  Should only be used when defining other required elements of a script's elements, such as
                 *  a script Constant.
                 */
                Translation();

                /**
                 *  Creates a rotation that affects the script's translation transformation.
                 *
                 * @param stackOperation how this translation should affect the translation stack of the script
                 * @param translation the amount to translate the script's translation transformation
                 */
                Translation(StackOperation stackOperation, glm::vec3 translation);

                /**
                 * Get the stack operation of the rotation.
                 *
                 * @return rotation's stack operation
                 */
                StackOperation getStackOperation() const;

                /**
                 * Get the translation that this Translation was initialized with.
                 *
                 * @return the initialized-with translation
                 */
                const glm::vec3& getTranslation() const;

                /**
                 * Get whether the Translation is active, as in it affects the script's translation transformation.
                 *
                 * If the Translation is default constructed, the returned value is false; otherwise true.
                 *
                 * @return true if the Translation modifies the script's translation transformation
                 */
                bool isEnabled() const;

            private:

                bool enabled;
                StackOperation stackOperation;
                glm::vec3 translation;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TRANSLATION_H
