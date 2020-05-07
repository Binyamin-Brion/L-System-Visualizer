//
// Created by binybrion on 5/6/20.
//

#include "Constant.h"

namespace L_System
{
    namespace DataStructures
    {
        Constant::Constant(QString constantName)
                    :
                        constantName{std::move(constantName)}
        {

        }

        Constant::Constant(Translation translation)
                    :
                        translation{translation}
        {

        }

        Constant::Constant(Rotation rotation)
                    :
                        rotation{rotation}
        {

        }

        Constant::Constant(Translation translation, Rotation rotation)
                    :
                        translation{translation},
                        rotation{rotation}
        {

        }

        const QString &Constant::getConstantName() const
        {
            return constantName;
        }

        const Rotation& Constant::getRotation() const
        {
            return rotation;
        }

        const Translation &Constant::getTranslation() const
        {
            return translation;
        }
    }
}