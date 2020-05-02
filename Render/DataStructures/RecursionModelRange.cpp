//
// Created by binybrion on 5/1/20.
//

#include "RecursionModelRange.h"

namespace Render
{
    namespace DataStructures
    {
        RecursionModelRange::RecursionModelRange(const QString &modelName, unsigned int instanceBeginIndex, unsigned int instanceCount)
                :
                    modelName{modelName},
                    transformationInstanceBegin{instanceBeginIndex},
                    transformationInstanceCount{instanceCount}
        {

        }

        void RecursionModelRange::deIncrementInstanceBegin(unsigned int amount)
        {
            transformationInstanceBegin -= amount;
        }

        const QString &RecursionModelRange::getModelName() const
        {
            return modelName;
        }

        unsigned int RecursionModelRange::getTransformationInstanceBegin() const
        {
            return transformationInstanceBegin;
        }

        unsigned int RecursionModelRange::getTransformationInstanceCount() const
        {
            return transformationInstanceCount;
        }

        void RecursionModelRange::setTransformationInstanceBegin(unsigned int value)
        {
            transformationInstanceBegin = value;
        }
    }
}