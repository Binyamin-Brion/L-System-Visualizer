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
        class Translation
        {
            public:
                Translation();
                Translation(StackOperation stackOperation, glm::vec3 translation);

                StackOperation getStackOperation() const;
                const glm::vec3& getTranslation() const;

                bool isEnabled() const;

            private:

                bool enabled;
                StackOperation stackOperation;
                glm::vec3 translation;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TRANSLATION_H
