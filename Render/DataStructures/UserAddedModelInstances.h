//
// Created by binybrion on 5/14/20.
//

#ifndef VOXEL_L_SYSTEM_USERADDEDMODELINSTANCES_H
#define VOXEL_L_SYSTEM_USERADDEDMODELINSTANCES_H

#include <QString>
#include <vector>

namespace Render
{
    namespace DataStructures
    {
        /**
         * Bundles together the indexes into the instance matrix VBO data for a given model.
         */

        struct UserAddedModelInstances
        {
            QString modelName;
            std::vector<unsigned int> instanceMatrixInstancesIndexes;
        };
    }
}

#endif //VOXEL_L_SYSTEM_USERADDEDMODELINSTANCES_H
