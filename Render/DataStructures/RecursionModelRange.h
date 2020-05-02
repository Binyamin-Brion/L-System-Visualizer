//
// Created by binybrion on 5/1/20.
//

#ifndef VOXEL_L_SYSTEM_RECURSIONMODELRANGE_H
#define VOXEL_L_SYSTEM_RECURSIONMODELRANGE_H


#include <QtCore/QString>

namespace Render
{
    namespace DataStructures
    {
        /**
         * Holds the indexes of the subset used for rendering of a model for a specific recursion depth in the RecursionTree.
         * This class itself is not used for rendering, but rather to keep track what parts of the vectors used for rendering
         * belong to what recursion depth level.
         */

        class RecursionModelRange
        {
            public:

                /**
                 * Initializes this range with the information used to identify the model and the subset fo data used for
                 * rendering instances of the model.
                 *
                 * @param modelName name of the file used to load the model
                 * @param instanceBeginIndex the beginning index of the an instance vector containing data to instance render the model
                 *                           for a certain recursion depth
                 * @param instanceCount the number number of elements in instance vector, starting from instanceBeginIndex, used to instance
                 *                      render the model for a certain recursion depth
                 */
                RecursionModelRange(const QString &modelName, unsigned int instanceBeginIndex, unsigned int instanceCount);

                /**
                 * Deincrements the begin index by the specified amount. Used if a model stored before this instance
                 * (so a model at a lower recursion depth, or a model uploaded before this one at the same recursion depth)
                 * is removed from the recursion tree.
                 *
                 * @param amount to deincrement the beginning index
                 */
                void deIncrementInstanceBegin(unsigned int amount);

                /**
                 * Get the name of the file representing the model held by this range.
                 *
                 * @return name of file used for loading the model represented by this range
                 */
                const QString& getModelName() const;

                /**
                 * Get the instance begin variable.
                 *
                 * @return current value of the instance begin variable
                 */
                unsigned int getTransformationInstanceBegin() const;

                /**
                 * Get the instance count variable.
                 *
                 * @return current value of the instance count variable
                 */
                unsigned int getTransformationInstanceCount() const;

                /**
                 * Set the value of the instance begin variable.
                 *
                 * @param value that the instance begin variable should have
                 */
                void setTransformationInstanceBegin(unsigned int value);
            private:

                QString modelName;
                unsigned int transformationInstanceBegin;
                unsigned int transformationInstanceCount;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RECURSIONMODELRANGE_H
