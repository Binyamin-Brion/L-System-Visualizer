//
// Created by binybrion on 5/6/20.
//

#include "Translation.h"

namespace L_System
{
    namespace DataStructures
    {
        Translation::Translation()
                        :
                            enabled{false}
        {

        }

        Translation::Translation(StackOperation stackOperation, glm::vec3 translation)
                        :
                            stackOperation{stackOperation},
                            translation{translation}
        {

        }

        StackOperation Translation::getStackOperation() const
        {
            return stackOperation;
        }

        const glm::vec3 &Translation::getTranslation() const
        {
            return translation;
        }

        bool Translation::isEnabled() const
        {
            return enabled;
        }
    }
}