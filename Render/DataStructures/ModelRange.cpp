//
// Created by binybrion on 5/1/20.
//

#include "ModelRange.h"

namespace Render
{
    namespace DataStructures
    {
        ModelRange::ModelRange(const ::ModelLoading::Model &model,
                               unsigned int instanceMatrixBegin, unsigned int instanceMatrixCount,
                               unsigned int indiceBegin, unsigned int indiceCount)
                               :
                                    model{model},
                                    instanceMatrixBegin{instanceMatrixBegin},
                                    instanceMatrixCount{instanceMatrixCount},
                                    indiceBegin{indiceBegin},
                                    indiceCount{indiceCount}
        {

        }

        void ModelRange::clearInstanceCount()
        {
            instanceMatrixCount= 0;

            userAddedMatrixCount = 0;
        }

        void ModelRange::deIncrementInstanceMatrixCount(unsigned int amount, bool userAddedIndex)
        {
            instanceMatrixCount -= amount;

            if(userAddedIndex)
            {
                userAddedMatrixCount -= amount;
            }
        }

        unsigned int ModelRange::getIndiceBegin() const
        {
            return indiceBegin;
        }

        unsigned int ModelRange::getIndiceCount() const
        {
            return indiceCount;
        }

        unsigned int ModelRange::getInstanceMatrixBegin() const
        {
            return instanceMatrixBegin;
        }

        unsigned int ModelRange::getInstanceMatrixCount() const
        {
            return instanceMatrixCount;
        }

        unsigned int ModelRange::getUserAddedMatrixCount() const
        {
            return userAddedMatrixCount;
        }

        void ModelRange::incrementInstanceMatrixCount(unsigned int amount, bool indexUserAdded)
        {
            instanceMatrixCount += amount;

            if(indexUserAdded)
            {
                userAddedMatrixCount += amount;
            }
        }

        bool ModelRange::indexUserAdded(unsigned int index) const
        {
            // User added instances are always added after the instances added by the interpretation of a script.
            // instanceMatrixCount holds the total number of instances, thus subtracting the userAddedMatrixCount determines
            // the number of instances that were added by the interpretation of the L-Script. Remember indexes start at 0!
            return (instanceMatrixBegin + instanceMatrixCount - userAddedMatrixCount) <= index && index < (instanceMatrixBegin + instanceMatrixCount);
        }

        const ::ModelLoading::Model &ModelRange::getModel() const
        {
            return model;
        }

        void ModelRange::setInstanceMatrixBegin(unsigned int value)
        {
            instanceMatrixBegin = value;
        }
    }
}
