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

        Constant::Constant(QString constantName, Translation translation)
                    :
                        constantName{std::move(constantName)},
                        translation{translation}
        {

        }

        Constant::Constant(QString constantName, Rotation rotation)
                    :
                        constantName{std::move(constantName)},
                        rotation{rotation}
        {

        }

        Constant::Constant(QString constantName, Translation translation, Rotation rotation)
                    :
                        constantName{std::move(constantName)},
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