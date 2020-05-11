//
// Created by binybrion on 5/10/20.
//

#ifndef VOXEL_L_SYSTEM_RULELOADING_H
#define VOXEL_L_SYSTEM_RULELOADING_H

#include "L_System/DataStructures/Variable/Variable.h"
#include "L_System/Execution/Token.h"
#include <vector>

namespace HelperFunctions
{
    /**
     * Finds the Variable data structure that has the same name as the name passed in.
     *
     * @param predecessorName name to match against a Variable
     * @param variables list of variables, one of which contains the name passed in
     * @return Variable that has the same name as what was passed in
     */
    ::L_System::DataStructures::Variable findEquivalentVariable(const QString &predecessorName, const std::vector<::L_System::DataStructures::Variable> &variables);

    /**
     * Creates a token using the Variable or Constant that has the same name as the name passed in.
     *
     * @param productionName name to match against a Variable or a Constant
     * @param constants list of constants that may have a Constant which has the name passed in
     * @param variables list of variables that may have a Constant which has the name passed in
     * @return Token created using the matched Variable or Constant
     */
    ::L_System::Execution::Token findEquivalentSuccessorTokens(const QString &productionName,
                                                                const std::vector<::L_System::DataStructures::Constant> &constants,
                                                                const std::vector<::L_System::DataStructures::Variable> &variables);
}

#endif //VOXEL_L_SYSTEM_RULELOADING_H
