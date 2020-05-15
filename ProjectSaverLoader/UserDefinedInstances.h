//
// Created by binybrion on 5/14/20.
//

#ifndef VOXEL_L_SYSTEM_USERDEFINEDINSTANCES_H
#define VOXEL_L_SYSTEM_USERDEFINEDINSTANCES_H

#include <QString>
#include <vector>
#include <mat4x4.hpp>

namespace ProjectSaverLoader
{
    /**
     * Bundles together the instance matrices added by the user for a given model.
     * These are stored in each favourite result that has user added models.
     */

    struct UserDefinedInstances
    {
        QString modelName;
        std::vector<glm::mat4x4> transformationMatrices;
    };
}

#endif //VOXEL_L_SYSTEM_USERDEFINEDINSTANCES_H
