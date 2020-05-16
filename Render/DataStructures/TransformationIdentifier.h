//
// Created by binybrion on 5/13/20.
//

#ifndef VOXEL_L_SYSTEM_TRANSFORMATIONIDENTIFIER_H
#define VOXEL_L_SYSTEM_TRANSFORMATIONIDENTIFIER_H

namespace Render
{
    namespace DataStructures
    {
        /**
         * Represents a transformation that can be applied to the selected model instances.
         */

        enum class TransformationIdentifier
        {
            RotationX,
            RotationY,
            RotationZ,

            ScaleX,
            ScaleY,
            ScaleZ,

            TranslationX,
            TranslationY,
            TranslationZ
        };
    }
}

#endif //VOXEL_L_SYSTEM_TRANSFORMATIONIDENTIFIER_H
