//
// Created by binybrion on 5/1/20.
//

#include "StoredModels.h"

namespace Render
{
    namespace DataStructures
    {
        void StoredModels::addModel(const ModelLoading::Model &model)
        {
            unsigned int indiceCount = 0;

            // The number of indices in a model is the sum of indices in the model. This operation needs to be done before
            // adding the model range. See StoredModels.h for reasoning for numberIndicesStored.
            for(const auto &mesh : model.getMeshes())
            {
                indiceCount += mesh.getIndices().size();
            }

            storedModels.push_back(ModelRange{model, 0, 0, numberIndicesStored, numberIndicesStored + indiceCount});

            // Now the actual number of indices can be updated.
            numberIndicesStored += indiceCount;
        }

        void StoredModels::addModelInstances(const QString &modelFileName, unsigned int numberInstances)
        {
            auto modelLocation = std::find_if(storedModels.begin(), storedModels.end(), [&modelFileName](const ModelRange &storedModel)
            {
                return storedModel.getModel().getModelFileName() == modelFileName;
            });

            // Protects data integrity. Before instances of the model can be added, the model itself needs to be added.
            if(modelLocation == storedModels.end())
            {
                assert(false);
            }

            modelLocation->incrementInstanceMatrixCount(numberInstances);

            updateIndiceIndexes();
        }

        bool StoredModels::checkIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection, const glm::mat4x4 &matrix, unsigned int index) const
        {
            // Checking against all models can be slow. To speed things up, only the model that is associated with the passed in matrix,
            // based off of the index, is checked.
            for(const auto &i : storedModels)
            {
               bool passFirstIndexTest = i.getInstanceMatrixBegin() <= index;
               bool passSecondIndexTest = index < (i.getInstanceMatrixBegin() + i.getInstanceMatrixCount());

               if(passFirstIndexTest && passSecondIndexTest)
               {
                   return i.getModel().checkIntersection(cameraPosition, rayDirection, matrix);
               }
            }

            return false;
        }

        const ModelRange &StoredModels::getModelRange(const QString &modelName) const
        {
            auto modelRangeLocation = std::find_if(storedModels.begin(), storedModels.end(), [&modelName](const ModelRange &heldRange)
            {
                return modelName == heldRange.getModel().getModelFileName();
            });

            // The model needs to be added before rendering range information can be returned.
            assert(modelRangeLocation != storedModels.end());

            return *modelRangeLocation;
        }

        const std::vector<ModelRange> &StoredModels::getModelRanges() const
        {
            return storedModels;
        }

        void StoredModels::updateIndiceIndexes()
        {
            unsigned int currentIndex = 0;

            for(auto &i : storedModels)
            {
                i.setInstanceMatrixBegin(currentIndex);

                currentIndex += i.getInstanceMatrixCount();
            }
        }

        void StoredModels::removeAllInstances()
        {
            storedModels.clear();

            numberIndicesStored = 0;
        }

        void StoredModels::removeModel(const QString &modelFileName)
        {
            auto modelLocation = std::find_if(storedModels.begin(), storedModels.end(), [&modelFileName](const ModelRange &storedModel)
            {
                return storedModel.getModel().getModelFileName() == modelFileName;
            });

            // Protects data integrity. The model has to exist for it to be removed.
            if(modelLocation == storedModels.end())
            {
                assert(false);
            }

            modelLocation->setInstanceMatrixBegin(0);
            modelLocation->incrementInstanceMatrixCount(-modelLocation->getInstanceMatrixCount());

            updateIndiceIndexes();
        }
    }
}