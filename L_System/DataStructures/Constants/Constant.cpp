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

        Constant::Constant(QString constantName, StackOperation stackOperation, Translation translation)
                    :
                        constantName{std::move(constantName)},
                        stackOperation{stackOperation},
                        translation{translation}
        {

        }

        Constant::Constant(QString constantName, StackOperation stackOperation, Rotation rotation)
                    :
                        constantName{std::move(constantName)},
                        stackOperation{stackOperation},
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

        StackOperation Constant::getStackOperation() const
        {
            return stackOperation;
        }

        const Translation &Constant::getTranslation() const
        {
            return translation;
        }
    }
}