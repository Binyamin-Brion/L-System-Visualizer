//
// Created by binybrion on 5/10/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTINFORMATION_H
#define VOXEL_L_SYSTEM_SCRIPTINFORMATION_H

#include <vector>
#include "L_System/DataStructures/Rules/Rule.h"
#include "L_System/DataStructures/Variable/Variable.h"
#include "FavouriteResult.h"

namespace ProjectSaverLoader
{
    /**
     * Combines the information about a single script into one entity.
     */

    struct ScriptInformation
    {
       //  std::vector<FavouriteResult> favouriteResults;

         ::L_System::DataStructures::Variable axiom;
         std::vector<::L_System::DataStructures::Constant> constants;
         std::vector<::L_System::DataStructures::Rule> rules;
         std::vector<::L_System::DataStructures::Variable> variables;
    };
}

#endif //VOXEL_L_SYSTEM_SCRIPTINFORMATION_H
