//
// Created by binybrion on 5/9/20.
//

#ifndef VOXEL_L_SYSTEM_FAVOURITERESULT_H
#define VOXEL_L_SYSTEM_FAVOURITERESULT_H

#include <vector>
#include "L_System/Execution/Token.h"
#include "UserDefinedInstances.h"

namespace ProjectSaverLoader
{
    /**
     * Represents a result of an execution that the user has saved.
     */

    struct FavouriteResult
    {
        QString resultName;
        std::vector<std::vector<::L_System::Execution::Token>> executionResult;
        std::vector<UserDefinedInstances> userDefinedInstances;
    };
}

#endif //VOXEL_L_SYSTEM_FAVOURITERESULT_H
