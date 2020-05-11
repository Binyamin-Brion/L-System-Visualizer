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

        Translation::Translation(glm::vec3 translation)
                        :
                            translation{translation},
                            enabled{true}
        {

        }

        const glm::vec3 &Translation::getTranslation() const
        {
            return translation;
        }

        bool Translation::isEnabled() const
        {
            return enabled;
        }

        bool Translation::operator==(const Translation &other) const
        {
            return translation == other.translation;
        }
    }
}