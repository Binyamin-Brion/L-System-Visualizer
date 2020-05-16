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

        void StoredModels::addModelInstances(const QString &modelFileName, unsigned int numberInstances, bool userAddedIndex)
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

            modelLocation->incrementInstanceMatrixCount(numberInstances, userAddedIndex);

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
                   // Only instances that were added by the user can be selected.
                   if(!i.indexUserAdded(index))
                   {
                       return false;
                   }

                   return i.getModel().checkIntersection(cameraPosition, rayDirection, matrix);
               }
            }

            return false;
        }

        QString StoredModels::getModelName(unsigned int index) const
        {
            for(const auto &i : storedModels)
            {
                if(i.getInstanceMatrixBegin() <= index && index <= (i.getInstanceMatrixBegin() + i.getInstanceMatrixCount()))
                {
                    return i.getModel().getModelFileName();
                }
            }

            assert(false && "\n\nNo model associated with the given index\n\n");
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

        std::vector<UserAddedModelInstances> StoredModels::getUserAddedMatricesIndexes() const
        {
            std::vector<UserAddedModelInstances> indexes;

            for(const auto &i : storedModels)
            {
                // If the associated model does not have any instances, then the index cannot refer to that model.
                if(i.getUserAddedMatrixCount() == 0)
                {
                    continue;
                }

                indexes.emplace_back(UserAddedModelInstances{i.getModel().getModelFileName(), std::vector<unsigned int>{}});

                // Find the starting index of the instances of the associated model.
                unsigned int baseIndex = i.getInstanceMatrixBegin() + i.getInstanceMatrixCount() - i.getUserAddedMatrixCount();

                // Add the indexes representing instances of the associated model.
                for(unsigned int index = 0; index < i.getUserAddedMatrixCount(); ++index)
                {
                    indexes.back().instanceMatrixInstancesIndexes.push_back(baseIndex + index);
                }
            }

            return indexes;
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

        void StoredModels::removeInstance(unsigned int index)
        {
            for(auto &i : storedModels)
            {
                // If the associated model does not have any instances, then the index cannot refer to that model.
                if(i.getInstanceMatrixCount() == 0)
                {
                    continue;
                }

                // Determine if the index is within the range of indexes that represent instances of a model
                unsigned int lowerRange = i.getInstanceMatrixBegin();
                unsigned int higherRange = i.getInstanceMatrixBegin() + i.getInstanceMatrixCount();

                bool targetModelInstances = (lowerRange <= index) && (index < higherRange);

                if(targetModelInstances)
                {
                    i.deIncrementInstanceMatrixCount(1, i.indexUserAdded(index));
                }
            }

            updateIndiceIndexes();
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
            modelLocation->clearInstanceCount();

            updateIndiceIndexes();
        }
    }
}