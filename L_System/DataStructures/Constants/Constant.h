//
// Created by binybrion on 5/5/20.
//

#ifndef VOXEL_L_SYSTEM_CONSTANT_H
#define VOXEL_L_SYSTEM_CONSTANT_H

#include <QtCore/QString>
#include <vec3.hpp>
#include "Translation.h"
#include "Rotation.h"

namespace L_System
{
    namespace DataStructures
    {
        class Constant
        {
            public:
                Constant() = default;
                explicit Constant(QString constantName);
                explicit Constant(Translation translation);
                explicit Constant(Rotation rotation);
                Constant(Translation translation, Rotation rotation);

                const QString& getConstantName() const;
                const Rotation& getRotation() const;
                const Translation& getTranslation() const;

            private:

                QString constantName;
                Translation translation;
                Rotation rotation;
        };
    }
}

#endif //VOXEL_L_SYSTEM_CONSTANT_H
