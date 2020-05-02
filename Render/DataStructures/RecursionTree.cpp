//
// Created by binybrion on 5/1/20.
//

#include <cassert>
#include "RecursionTree.h"

namespace Render
{
    namespace DataStructures
    {
        void RecursionTree::addModelRecursiveResult(unsigned int recursionDepth, RecursionModelRange model)
        {
            // If a recursion level is specified that is higher than any other previous level, then add the appropriate
            // amount of vectors (each represents a recursion level) until the recursion level passed in can be represented

            if(recursionDepth > recursionDepthModels.size())
            {
                for(unsigned int i = recursionDepthModels.size(); i < recursionDepth; ++i)
                {
                    recursionDepthModels.emplace_back();
                }
            }

            recursionDepthModels[recursionDepth - 1].emplace_back(model);
        }

        void RecursionTree::removeModel(const RecursionModelRange &model)
        {
            // Search through all of the recursion levels and remove any instances of the passed in model. Since no tracking
            // is done to know what recursion levels contain the passed in model, all of the recursion levels have to be searched.
            for(auto &i : recursionDepthModels)
            {
                removeModel(i, model);
            }

            adjustIndexes(model.getTransformationInstanceBegin(), model.getTransformationInstanceCount());
        }

        RemoveModelRecursionResult RecursionTree::removeModelRecursiveResult(unsigned int recursionDepth, const QString &modelFileName)
        {
            assert( (recursionDepth - 1) < recursionDepthModels.size());

            // Find where the model is in the recursion level. If none is found, then it is assumed a non-fatal error occured and no action is taken.

            auto modelLocation = std::find_if(recursionDepthModels[recursionDepth - 1].begin(), recursionDepthModels[recursionDepth - 1].end(), [&modelFileName](const RecursionModelRange &heldModel)
            {
                return heldModel.getModelName() == modelFileName;
            });

            if(modelLocation == recursionDepthModels[recursionDepth - 1].end())
            {
                return {false, 0, 0};
            }

            // Keep track of the result to return, as this won't be available after the model is removed from the recursion vector.
            std::pair<unsigned int, unsigned int> returnResult{modelLocation->getTransformationInstanceBegin(), modelLocation->getTransformationInstanceCount()};

            recursionDepthModels[recursionDepth - 1].erase(modelLocation);

            // The indexes associated with the remaining models have to be updated to reflect the fact that the instance matrices vector,
            // which they represnts, has changed due to a model (and its associated instanced matrices being removed).
            updateIndexes();

            return {true, returnResult.first, returnResult.second};
        }

        // Beginning of private functions

        void RecursionTree::adjustIndexes(unsigned int beginIndex, unsigned int count)
        {
            for(auto &i : recursionDepthModels)
            {
                for(auto &j : i)
                {
                    if(j.getTransformationInstanceBegin() > beginIndex)
                    {
                        j.deIncrementInstanceBegin(count);
                    }
                }
            }
        }

        void RecursionTree::removeModel(std::vector<RecursionModelRange> &vector, const RecursionModelRange &model)
        {
            std::remove_if(vector.begin(), vector.end(), [&model](const RecursionModelRange &heldModel)
            {
                return model.getModelName() == heldModel.getModelName();
            });
        }

        void RecursionTree::updateIndexes()
        {
            // Iterate over all of the models, and rebuild the indexes they store.
            unsigned int currentIndex = 0;

            for(auto &i : recursionDepthModels)
            {
                for(auto &j : i)
                {
                    j.setTransformationInstanceBegin(currentIndex);

                    currentIndex += j.getTransformationInstanceCount();
                }
            }
        }
    }
}