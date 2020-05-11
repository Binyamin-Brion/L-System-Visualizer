//
// Created by binybrion on 5/9/20.
//

#ifndef VOXEL_L_SYSTEM_FAVOURITERESULT_H
#define VOXEL_L_SYSTEM_FAVOURITERESULT_H

#include <vector>
#include "L_System/Execution/Token.h"

namespace ProjectSaverLoader
{
    struct FavouriteResult
    {
        QString resultName;
        std::vector<std::vector<::L_System::Execution::Token>> executionResult;
    };
}

#endif //VOXEL_L_SYSTEM_FAVOURITERESULT_H
