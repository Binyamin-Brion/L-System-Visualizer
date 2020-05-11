//
// Created by binybrion on 5/10/20.
//

#include <cassert>
#include "RuleLoading.h"

namespace HelperFunctions
{
    ::L_System::DataStructures::Variable findEquivalentVariable(const QString &predecessorName, const std::vector<::L_System::DataStructures::Variable> &variables)
    {
        // Check which variable matches the starting variable of the rule, and use it to create the Rule along with the
        // matched predecessor tokens in the above loop.
        auto variableLocation = std::find_if(variables.begin(), variables.end(), [&predecessorName](const ::L_System::DataStructures::Variable &variable)
        {
            return variable.getVariableName() == predecessorName;
        });

        if(variableLocation != variables.end())
        {
            return *variableLocation;
        }

        // A starting variable must be one of the declared variables; it is an error if this is not the case.
        assert(false);
    }

    ::L_System::Execution::Token findEquivalentSuccessorTokens(const QString &productionName,
                                                               const std::vector<::L_System::DataStructures::Constant> &constants,
                                                               const std::vector<::L_System::DataStructures::Variable> &variables)
    {
        // Check if the production is one of the variables, and if it is, add it to the rule.
        auto variableLocation = std::find_if(variables.begin(), variables.end(), [&](const ::L_System::DataStructures::Variable &variable)
        {
            return variable.getVariableName() == productionName;
        });

        if(variableLocation != variables.end())
        {
            return ::L_System::Execution::Token{*variableLocation};
        }

        // Check if the production is one of the constants, and if it is, add it to the rule.
        auto constantLocations = std::find_if(constants.begin(), constants.end(), [&](const ::L_System::DataStructures::Constant &constant)
        {
            return constant.getConstantName() == productionName;
        });

        if(constantLocations != constants.end())
        {
            return ::L_System::Execution::Token{*constantLocations};
        }

        // A predecessor token must be either a variable or a constant; it is an error if it is neither.
        assert(false);
    }
}