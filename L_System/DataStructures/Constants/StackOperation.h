//
// Created by binybrion on 5/5/20.
//

#ifndef VOXEL_L_SYSTEM_STACKOPERATION_H
#define VOXEL_L_SYSTEM_STACKOPERATION_H

namespace L_System
{
    namespace DataStructures
    {
        /**
         * Represents the possible stack operations that constant can perform on a L-Script's
         * rotation or transformation stack.
         */

        enum class StackOperation
        {
            NoAction,
            Pop,
            Push
        };
    }
}

#endif //VOXEL_L_SYSTEM_STACKOPERATION_H
