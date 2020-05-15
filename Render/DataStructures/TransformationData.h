//
// Created by binybrion on 5/13/20.
//

#ifndef VOXEL_L_SYSTEM_TRANSFORMATIONDATA_H
#define VOXEL_L_SYSTEM_TRANSFORMATIONDATA_H

#include "TransformationIdentifier.h"

namespace Render
{
    namespace DataStructures
    {
        /**
         * Represents the transformation to apply to selected model instances, as well as the magnitude of that transformation.
         */

        struct TransformationData
        {
            TransformationIdentifier transformationIdentifier;
            float amount;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TRANSFORMATIONDATA_H
