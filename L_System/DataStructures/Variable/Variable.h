//
// Created by binybrion on 5/5/20.
//

#ifndef VOXEL_L_SYSTEM_VARIABLE_H
#define VOXEL_L_SYSTEM_VARIABLE_H

#include <QString>

namespace L_System
{
    namespace Execution
    {
        class Token;
    }

    namespace DataStructures
    {
        /**
         * Represents a variable in an L-Script, that can be matched against a Rule to modify the
         * script's recursive result.
         */

        class Variable
        {
            public:

                /**
                 * Default constructed variable that has no name nor associated model.
                 *
                 * Should only be used when defining other required elements of a script's elements, such as the
                 * Tokens for the scripts Executor.
                 */
                Variable() = default;

                /**
                 * Initializes the variable with  name and an associated model.
                 *
                 * @param variableName name of the variable. This is what a Rule will be matched against
                 * @param associatedModel the model (to render) that this variable represents
                 */
                Variable(QString variableName, QString associatedModel);

                /**
                 * The name of the model used to initialize this Variable. It may be the location of the file used to load
                 * the model.
                 *
                 * @return initialized with name of model
                 */
                const QString &getAssociatedModelName() const;

                /**
                 * Name of the variable ued to initialize this Variable.
                 *
                 * @return initialized with variable name
                 */
                const QString &getVariableName() const;

                /**
                 * Checks if two Variables are equal to each other.
                 *
                 * @param other Variable to compare against this one
                 * @return true if this variable's and the other variable's name and associatedModel BOTH match
                 */
                bool operator==(const Variable &other) const;

            private:

                QString variableName;
                QString associatedModel;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLE_H
