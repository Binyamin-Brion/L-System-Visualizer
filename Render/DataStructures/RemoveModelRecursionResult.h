//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_REMOVEMODELRECURSIONRESULT_H
#define VOXEL_L_SYSTEM_REMOVEMODELRECURSIONRESULT_H

namespace Render
{
    namespace DataStructures
    {
        /**
         *  Keeps track of the result of trying to remove a model at a specified recursion depth.
         *  Used as a return result using RecursionTree.
         */

        struct RemoveModelRecursionResult
        {
            bool entryRemoved;
            unsigned int instanceMatrixBeginIndex;
            unsigned int instanceMatrixCount;
        };
    }
}

#endif //VOXEL_L_SYSTEM_REMOVEMODELRECURSIONRESULT_H
