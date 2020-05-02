//
// Created by binybrion on 5/1/20.
//

#ifndef VOXEL_L_SYSTEM_RECURSIONTREE_H
#define VOXEL_L_SYSTEM_RECURSIONTREE_H

#include <vector>
#include "RecursionModelRange.h"
#include "RemoveModelRecursionResult.h"

namespace Render
{
    namespace DataStructures
    {
        /**
         * Keeps track of the subsets into the instance matrix used for rendering for each recursion depth for a given model.
         */

        class RecursionTree
        {
            public:

                /**
                 * Add a model to keep track for a given recursion level.
                 *
                 * @param recursionDepth level of recursion that the passed in range represents
                 * @param model range with subset information into the instance matrix vector
                 */
                void addModelRecursiveResult(unsigned int recursionDepth, RecursionModelRange model);

                /**
                 * Removes all instances of the passed in model from the recursion tree.
                 *
                 * @param model to remove from the recursion tree
                 */
                void removeModel(const RecursionModelRange &model);

                /**
                 * Removes the specified model from the given recursion level. If no such model exists at the recursion level,
                 * no action is taken.
                 *
                 * @param recursionDepth to remove from
                 * @param modelFileName identifying the model to remove
                 * @return result indicating the result of the requested operation
                 */
                RemoveModelRecursionResult removeModelRecursiveResult(unsigned int recursionDepth, const QString &modelFileName);

            private:

                /**
                 * Adjust the indexes stored in the ranges as a result of removing a model from a recursion level(s).
                 *
                 * @param beginIndex the begin index into the instance matrix vector of the model that was removed
                 * @param count the count instance matrices of the model that was removed
                 */
                void adjustIndexes(unsigned int beginIndex, unsigned int count);

                /**
                 * Checks if the passed in model is in the passed in vector, and if it is, it is removed from the vector.
                 *
                 * @param vector to search for the passed in model
                 * @param model to be removed
                 */
                void removeModel(std::vector<RecursionModelRange> &vector, const RecursionModelRange &model);

                /**
                 * Adjust the indexes stored in the ranges as a result of removing a model from a recursion level(s).
                 */
                void updateIndexes();

                std::vector<std::vector<RecursionModelRange>> recursionDepthModels;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RECURSIONTREE_H
