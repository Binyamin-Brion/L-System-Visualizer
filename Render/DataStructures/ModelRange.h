//
// Created by binybrion on 5/1/20.
//

#ifndef VOXEL_L_SYSTEM_MODELRANGE_H
#define VOXEL_L_SYSTEM_MODELRANGE_H

#include "ModelLoading/Model.h"

namespace Render
{
    namespace DataStructures
    {
        /**
         *  Holds indexes of subsets used for instance rendering a model. These are stored in the StoredModels class.
         */

        class ModelRange
        {
            public:

                /**
                 * Initializes this range with the information required to instance render a model.
                 *
                 * @param model name of the file used to load the model this class instance will represent
                 * @param instanceMatrixBegin starting index into the instance vector for instance matrices for the associated model
                 * @param instanceMatrixCount number of instance matrices for the associated model, starting from instanceMatrixBegin
                 * @param indiceBegin starting index into the indice vector for the indices of the associated model
                 * @param indiceCount number of indices required for rendreing the model, starting from indiceBegin
                 */
                ModelRange(const ::ModelLoading::Model &model,
                        unsigned int instanceMatrixBegin, unsigned int instanceMatrixCount,
                        unsigned int indiceBegin, unsigned int indiceCount);

                /**
                 * Deincrements the count of the instance matrices for the associated model. Called if instances of the
                 * associated model are no longer to be rendered.
                 *
                 * @param amount number of instances of associated model to stop rendering
                 */
                void deIncrementInstanceMatrixCount(unsigned int amount);

                /**
                 * Get the starting index into the indice vector for rendering.
                 *
                 * @return starting index into indice vector
                 */
                unsigned int getIndiceBegin() const;

                /**
                 * Get the number of indices used for rendering the associated model.
                 *
                 * @return number of indices to render
                 */
                unsigned int getIndiceCount() const;

                /**
                 * Get the starting index into the instance matrix vector for rendering.
                 *
                 * @return starting index into the instance matrix vector
                 */
                unsigned int getInstanceMatrixBegin() const;

                /**
                 * Get the number of instances to render the associated model.
                 *
                 * @return number of instances to render
                 */
                unsigned int getInstanceMatrixCount() const;

                /**
                 * Get the name of the file representing the model held by this range.
                 *
                 * @return name of file used for loading the model represented by this range
                 */
                const ::ModelLoading::Model& getModel() const;

                /**
                 * Increment the number of instances of the associated model to render.
                 *
                 * @param amount number of additional instances
                 */
                void incrementInstanceMatrixCount(unsigned int amount);

                /**
                 * Set the beginning index into the instance matrix vector. Called when a model preceding this one
                 * (determined based off of order of loading) is has its number of instances to render changed.
                 *
                 * @param value new index into instance matrix vector
                 */
                void setInstanceMatrixBegin(unsigned int value);

            private:

                ::ModelLoading::Model model;
                unsigned int instanceMatrixBegin;
                unsigned int instanceMatrixCount;
                unsigned int indiceBegin;
                unsigned int indiceCount;
        };
    }
}

#endif //VOXEL_L_SYSTEM_MODELRANGE_H
