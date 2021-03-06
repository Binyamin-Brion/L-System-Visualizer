//
// Created by binybrion on 5/10/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTINFORMATION_H
#define VOXEL_L_SYSTEM_SCRIPTINFORMATION_H

#include <vector>
#include "L_System/DataStructures/Rules/Rule.h"
#include "L_System/DataStructures/Variable/Variable.h"
#include "FavouriteResult.h"
#include "UserDefinedInstances.h"

namespace ProjectSaverLoader
{
    /**
     * Combines the information about a single script into one entity.
     */

    struct ScriptInformation
    {
         QString scriptName;
         ::L_System::DataStructures::Variable axiom;
         std::vector<::L_System::DataStructures::Constant> constants;
         std::vector<::L_System::DataStructures::Rule> rules;
         std::vector<::L_System::DataStructures::Variable> variables;
         std::vector<FavouriteResult> favouriteResults;
    };
}

#endif //VOXEL_L_SYSTEM_SCRIPTINFORMATION_H
