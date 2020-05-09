//
// Created by binybrion on 5/6/20.
//

#include "Variable.h"

namespace L_System
{
    namespace DataStructures
    {
        Variable::Variable(QString variableName, QString associatedModel)
                    :
                        variableName{std::move(variableName)},
                        associatedModel{std::move(associatedModel)}
        {

        }

        const QString &Variable::getAssociatedModelName() const
        {
            return associatedModel;
        }

        const QString &Variable::getVariableName() const
        {
            return variableName;
        }

        bool Variable::operator==(const Variable &other) const
        {
            return variableName == other.variableName && associatedModel == other.associatedModel;
        }
    }
}