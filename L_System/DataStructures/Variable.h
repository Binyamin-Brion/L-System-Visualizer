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
        class Variable
        {
            public:

                Variable() = default;
                Variable(QString variableName, QString associatedModel);

                const QString &getAssociatedModelName() const;
                const QString &getVariableName() const;

                bool operator==(const Variable &other) const;

            private:

                QString variableName;
                QString associatedModel;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLE_H
