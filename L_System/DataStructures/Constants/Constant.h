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
        /**
         * Represents a constant used in an L-Script. Has the ability to manipulate the script's translation and rotation
         * as well as pushing or popping the script's stack.
         */

        class Constant
        {
            public:

                /**
                 * Default constructed constant that has no name and does not manipulate the script's transformation.
                 *
                 * Should only be used when defining other required elements of a script's elements, such as the
                 * Tokens for the scripts Executor.
                 */
                Constant() = default;

                /**
                 * Constant with a name. The script's transformations are not affected, and the only thing this constant
                 * will contribute to the script is the output.
                 *
                 * Should only be used for testing.
                 *
                 * @param constantName name of the constant
                 */
                explicit Constant(QString constantName);

                /**
                 * Constant that has the ability to modify the script's translation transformation.
                 *
                 * @param constantName name of the the constant
                 * @param translation how the script's translation transformation should change when encountering this constant
                 */
                Constant(QString constantName, Translation translation);

                /**
                 * Constant that has the ability to modify the script's rotation transformation.
                 *
                 * @param constantName name of the the constant
                 * @param rotation how the script's rotation transformation should change when encountering this constant
                 */
                Constant(QString constantName, Rotation rotation);

                /**
                 * Constant that has the ability to modify the script's translation and rotation transformation.
                 *
                 * @param constantName name of the the constant
                 * @param translation how the script's translation transformation should change when encountering this constant
                 * @param rotation how the script's rotation transformation should change when encountering this constant
                 */
                Constant(QString constantName, Translation translation, Rotation rotation);

                /**
                 * Get the name of the constant used to initialize this constant.
                 *
                 * @return name of this constant
                 */
                const QString& getConstantName() const;

                /**
                 * Get the rotation that is used to modify a script's rotation transformation.
                 *
                 * @return rotation that modifies the script
                 */
                const Rotation& getRotation() const;

                /**
                 * Get the translation that is used to modify a script's translation transformation.
                 *
                 * @return translation that modifies the script
                 */
                const Translation& getTranslation() const;

            private:

                QString constantName;
                Translation translation;
                Rotation rotation;
        };
    }
}

#endif //VOXEL_L_SYSTEM_CONSTANT_H
