//
// Created by binybrion on 5/1/20.
//

#ifndef VOXEL_L_SYSTEM_STOREDMODELS_H
#define VOXEL_L_SYSTEM_STOREDMODELS_H

#include "ModelRange.h"
#include "UserAddedModelInstances.h"

namespace Render
{
    namespace DataStructures
    {
        /**
         * Stores all of the ranges into rendering data vectors for all of the loaded models. This allows the required
         * rendering data for a given model to be obtained in order to be rendered.
         */

        class StoredModels
        {
            public:

                /**
                 * Adds the specified model to the list of models tracked.
                 *
                 * @param model to have its rendering indexes stored
                 */
                void addModel(const ModelLoading::Model &model);

                /**
                 * Updates the number of instances of the specified model to be rendered.
                 *
                 * @param modelFileName the name of the file that was used to load the model
                 * @param numberInstances number of instances to add to the referred to model
                 * @param userAddedIndex whether the added instances are user-added
                 */
                void addModelInstances(const QString &modelFileName, unsigned int numberInstances, bool userAddedIndex);

                /**
                 * Checks for an intersection between a ray with an initial position and all of the instances of all models stored.
                 *
                 * @param cameraPosition position of the camera in world space when the intersection test takes place
                 * @param rayDirection direction of the eay from the camera position in world space when the test takes place
                 * @param matrix transformation matrix to convert the base instance of the model from local to world space
                 * @param index of the transformation matrix in the transformation matrix vector
                 * @return true if there was an intersection
                 */
                bool checkIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection, const glm::mat4x4 &matrix, unsigned int index) const;

                /**
                 * Get the model name associated with the instance represented by the passed in index.
                 *
                 * @param index for which to retrieve the associated model name
                 * @return instance's model file name (instance represented by the index)
                 */
                QString getModelName(unsigned int index) const;

                /**
                 * Get the object storing ranges into rendering data vectors of the associated model
                 *
                 * @param modelName name of the the file that was used to load the model
                 * @return object storing the required ranges for rendering the referred to model
                 */
                const ModelRange& getModelRange(const QString &modelName) const;

                /**
                 * Get all of objects storing range information about all of the added models.
                 *
                 * @return vector of objects storing all rendering ranges
                 */
                const std::vector<ModelRange>& getModelRanges() const;

                /**
                 * Get a vector of indexes into the transformation VBO data for each held model.
                 *
                 * @return vector of indexes into the transformation VBO data for each held model.
                 */
                std::vector<UserAddedModelInstances> getUserAddedMatricesIndexes() const;

                /**
                 * Removes the instance, regardless of the model, that is associated with the passed in index.
                 *
                 * @param index for which to remove from being rendered
                 */
                void removeInstance(unsigned int index);

                /**
                 * Remove all instances of the referred to model. Instances for the model can be added again without calling
                 * addModel() again.
                 *
                 * @param modelFileName name of the the file that was used to load the model
                 */
                void removeModel(const QString &modelFileName);

            private:

                /**
                 * Updates the rendering ranges for the stored object after instances were added or removed to a model.
                 */
                void updateIndiceIndexes();

                std::vector<ModelRange> storedModels;

                // Keep track of how the indices stored in the indices vector so that the required offsets into the
                // indices vector can be obtained when issuing draw calls.
                unsigned int numberIndicesStored = 0;
        };
    }
}

#endif //VOXEL_L_SYSTEM_STOREDMODELS_H
