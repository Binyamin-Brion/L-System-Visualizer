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

        void ModelRange::deIncrementInstanceMatrixCount(unsigned int amount)
        {
            instanceMatrixCount -= amount;
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

        void ModelRange::incrementInstanceMatrixCount(unsigned int amount)
        {
            instanceMatrixCount += amount;
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
