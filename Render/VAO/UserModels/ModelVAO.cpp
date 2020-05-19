//
// Created by binybrion on 4/30/20.
//

#include <ext/matrix_transform.hpp>
#include "ModelVAO.h"
#include "ModelLoading/Model.h"
#include "ProjectSaverLoader/ProjectDetails.h"
#include "../../Shaders/ShaderManager.h"

namespace Render
{
    namespace VAO
    {
        void ModelVAO::addModel(const ::ModelLoading::Model &model)
        {
            storedModels.addModel(model);

            // For some reason uploading the actual model data to vRam leads to a crash; therefore a copy of the
            // of the model is made and is uploaded in the next render loop.
            modelsToUpload.push_back(model);
        }

        void ModelVAO::addModelInstances(const QString &modelFileName, const std::vector<glm::mat4x4> &transformationMatrices, bool userAddedIndex)
        {
            // Add the transformation matrices to the required location in the instance matrices vector. This has to be done so that the
            // instance rendering will render only those instances associated with a particular model.
            auto instanceTransformationVector = transformationsVBO.getHeldData();

            unsigned int offset = storedModels.getModelRange(modelFileName).getInstanceMatrixBegin() + storedModels.getModelRange(modelFileName).getInstanceMatrixCount();

            instanceTransformationVector.insert(instanceTransformationVector.begin() + offset, transformationMatrices.begin(), transformationMatrices.end());

            transformationsVBO.uploadData(instanceTransformationVector);

            // Keep track of whether the instances being added are selected or not.
            std::vector<unsigned char> instancesSelected;

            // The default colour is set in the fragment shader. Adding false here will by default make the added instances
            // have that default colour.
            for(unsigned int i = 0; i < transformationMatrices.size(); ++i)
            {
                instancesSelected.emplace_back(false);
            }

            instanceColours.uploadDataAppend(instancesSelected);

            storedModels.addModelInstances(modelFileName, transformationMatrices.size(), userAddedIndex);

            intersectionIndexes.clear();
        }

        void ModelVAO::addUserRequestedModelInstance(const QString &modelFileName)
        {
            // Un-highlight any existing highlighted model instances, regardless of the model being rendered.
            auto instancesSelected = instanceColours.getHeldData();

            for(unsigned int index : intersectionIndexes)
            {
                instancesSelected[index] = false;
            }

            // Ensure that the fact that no instances are highlighted is shown in the render.
            instanceColours.uploadData(instancesSelected);

            // Logically mark the fact that no instances are selected at this point.
            intersectionIndexes.clear();

            addModelInstances(modelFileName, {glm::mat4{1.0f}}, true);

            // Highlight the added instance. Since it is known that the instance is added at the end of the VBO data slice associated with the
            // model of which an instance is added, it is known what part of the instance VBO data to update. The '- 1' is because indexes start at 0.
            unsigned int addedInstanceIndex = storedModels.getModelRange(modelFileName).getInstanceMatrixBegin() + storedModels.getModelRange(modelFileName).getInstanceMatrixCount() - 1;

            instancesSelected = instanceColours.getHeldData();

            // Logically mark the added instance as being selected.
            intersectionIndexes.push_back(addedInstanceIndex);

            instancesSelected[addedInstanceIndex] = true;

            // Ensure that the fact that no instances are highlighted is shown in the render.
            instanceColours.uploadData(instancesSelected);

            // Remember that an instance has been added.
            HistoryChange historyChange{modelFileName, addedInstanceIndex, true, false, glm::mat4x4{1.0f}};

            Modification modification{historyChange};

            historyChanges.push_back(modification);
        }

        void ModelVAO::addUserRequestedModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &modelInstances)
        {
            // All previous user history is forgotten; this simplifies implementation of the history.
            historyChanges.clear();

            for(const auto &i : modelInstances)
            {
                addModelInstances(i.modelName, i.transformationMatrices, true);
            }
        }

        void ModelVAO::checkRayIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection, bool appendIntersections)
        {
            /*
             * The highlighting behaviour is as follows:
             *
             * If there is an intersection:
             *
             *  1. The instance is not already highlighted (the case if it was not selected previously), then highlight it.
             *     Previous highlighted models are un-highlighted if the appendIntersections value is false.
             *
             *  2 If the instance is not already highlighted and the appendIntersections value is true, then highlight the
             *    intersected model while not un-highlighting the previous highlighted models.
             *
             *  3. The instance has already been highlighted, then un-highlight it.
             *
             *  If there is no intersection, then un-highlight the model if it was previously highlighted
             */

            for(unsigned int i = 0; i < transformationsVBO.getHeldData().size(); ++i)
            {
                bool intersectionOccurred = storedModels.checkIntersection(cameraPosition, rayDirection, transformationsVBO.getHeldData()[i], i);

                bool instanceAlreadyHighlighted = std::find(intersectionIndexes.begin(), intersectionIndexes.end(), i) != intersectionIndexes.end();

                bool removeHighlighting = false;

                if(intersectionOccurred) // Case 1, and part of 2
                {
                    if(!instanceAlreadyHighlighted) // Case 1
                    {
                        auto instanceColourVector = instanceColours.getHeldData();

                        instanceColourVector[i] = true;

                        instanceColours.uploadData(instanceColourVector);

                        intersectionIndexes.push_back(i);

                        continue; // Ensure that highlight does not get un-highlighted
                    }

                    removeHighlighting = true; // Beginning of case 2
                }

                if(!appendIntersections || removeHighlighting) // Case 3, and end of case 2
                {
                    // Only update the instance to the default colour if it is highlighted already.
                    auto intersectionIndex = std::find(intersectionIndexes.begin(), intersectionIndexes.end(), i);

                    if(instanceAlreadyHighlighted)
                    {
                        auto instanceColourVector = instanceColours.getHeldData();

                        instanceColourVector[i] = false;

                        instanceColours.uploadData(instanceColourVector);

                        intersectionIndexes.erase(intersectionIndex);
                    }
                }
            }
        }

        void ModelVAO::deleteOpenGLResources()
        {
            // Unbind the vao so that the VBOs are no longer referenced, ensuring that deleting the VBOs actually
            // causes the VBO resource to be released.

            glBindVertexArray(0);

            glDeleteVertexArrays(1, &vao);

            verticesVBO.deleteVBO();
            indices.deleteVBO();
            transformationsVBO.deleteVBO();
            instanceColours.deleteVBO();
        }

        void ModelVAO::deleteSelectedInstances()
        {
            // The copy of the transformation matrices being deleted have to be done before the instances are deleted!
            auto transformationVector = transformationsVBO.getHeldData();

            Modification modification;

            for(const auto &i : intersectionIndexes)
            {
                HistoryChange historyChange{storedModels.getModelName(i), i, false, true, transformationVector[i]};

                modification.push_back(historyChange);
            }

            historyChanges.push_back(modification);

            removeInstances(intersectionIndexes);
        }

        std::vector<::ProjectSaverLoader::UserDefinedInstances> ModelVAO::getUserDefinedInstances() const
        {
            std::vector<::ProjectSaverLoader::UserDefinedInstances> userDefinedInstances;

            // The stored models holds the indexes into the transformation matrices VBO data for the instances for each
            // model, but actual matrices are needed (which are stored in this object). Thus the below loop.
            for(const auto &i : storedModels.getUserAddedMatricesIndexes())
            {
                std::vector<glm::mat4x4> userInstancesMatrices;

                for(const auto &index : i.instanceMatrixInstancesIndexes)
                {
                    userInstancesMatrices.emplace_back(transformationsVBO.getHeldData()[index]);
                }

                userDefinedInstances.emplace_back(::ProjectSaverLoader::UserDefinedInstances{i.modelName, userInstancesMatrices});
            }

            return userDefinedInstances;
        }

        void ModelVAO::initialize()
        {
            if(!initializeOpenGLFunctions())
            {
                throw std::runtime_error{"Unable to initialize OpenGL functions at: " + std::string{__PRETTY_FUNCTION__}};
            }

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            indices.initialize();

            // Note: the following VBOs require a bind() call before the vertex attrib function calls as the function
            // that initializes the VBO does not bind them.

            verticesVBO.initialize();
            verticesVBO.bind();
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
            glEnableVertexAttribArray(0);

            // Remember that these are matrices being stored in this VBO, hence 4 layout locations are required.
            transformationsVBO.initialize();
            transformationsVBO.bind();

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), nullptr);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(1, 1);
            glVertexAttribDivisor(2, 1);
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);

            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);

            instanceColours.initialize();
            instanceColours.bind();

            glVertexAttribIPointer(5, 1, GL_UNSIGNED_BYTE, sizeof(unsigned char), nullptr);
            glVertexAttribDivisor(5, 1);
            glEnableVertexAttribArray(5);

            textureCoordinates.initialize();
            textureCoordinates.bind();
            glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
            glEnableVertexAttribArray(6);

            textureManager.initialize();
        }

        void ModelVAO::removeModelInstances(const QString &modelFileName)
        {
            // Not only does the model have to be removed logically so draw calls aren't issued, but the data associated
            // with the model held in the VBOs have to be removed as well.

            auto transformationsVector = transformationsVBO.getHeldData();

            auto instanceColoursVector = instanceColours.getHeldData();

            // Find the model associated information.

            auto models = storedModels.getModelRanges();

            auto modelLocation = std::find_if(models.begin(), models.end(), [&modelFileName](const DataStructures::ModelRange &modelRange)
            {
                return modelRange.getModel().getModelFileName() == modelFileName;
            });

            // Actually erase data.

            unsigned int offset = modelLocation->getInstanceMatrixBegin();

            unsigned int count = modelLocation->getInstanceMatrixCount();

            transformationsVector.erase(transformationsVector.begin() + offset, transformationsVector.begin() + offset + count);

            instanceColoursVector.erase(instanceColoursVector.begin() + offset, instanceColoursVector.begin() + offset + count);

            // Apply updated data to the VBOs.

            transformationsVBO.uploadData(transformationsVector);

            instanceColours.uploadData(instanceColoursVector);

            // Logically remove the model information.

            storedModels.removeModel(modelFileName);
        }

        void ModelVAO::render(Shader::ShaderManager &shaderManager)
        {
            glBindVertexArray(vao);

            uploadModel();

            // These were specified in the TextureManager; for example, the sampler for 512x512 textures was initialized
            // with the texture unit 0.
            shaderManager.getShaderProgram("UserModels").setUniformValue("textures_512x512", 0);
            shaderManager.getShaderProgram("UserModels").setUniformValue("textures_1024x1024", 1);
            shaderManager.getShaderProgram("UserModels").setUniformValue("texture_2048x2048", 2);

            // Instance render the required number of instances for each model.
            for(const auto &i : storedModels.getModelRanges())
            {
                // Specifies how many indices of the model have been processed before; this is required as the StoredModels
                // holds how many indices are required for the entire model, but due to textures, a model has to be broken
                // down into meshes, each with its own number of indices.
                unsigned int indicesProcessedSoFar = 0;

                for(const auto &mesh : i.getModel().getMeshes())
                {
                    Textures::TextureID textureId = textureManager.getTextureBank().getTextureID(QString::fromStdString(mesh.getTextureLocation()));

                    shaderManager.getShaderProgram("UserModels").setUniformValue("textureID", textureId.textureID);
                    shaderManager.getShaderProgram("UserModels").setUniformValue("offset", textureId.offset);

                    glDrawElementsInstancedBaseInstance(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * (i.getIndiceBegin() + indicesProcessedSoFar)),
                                                        i.getInstanceMatrixCount(), i.getInstanceMatrixBegin());

                    indicesProcessedSoFar += mesh.getIndices().size();
                }
            }
        }

        void ModelVAO::transformSelectedModels(const DataStructures::TransformationData &transformationData)
        {
            auto instanceTransformationVector = transformationsVBO.getHeldData();

            Modification modification;

            for(unsigned int index : intersectionIndexes)
            {
                // The copy of the transformation matrix has to be done before the transformation is applied.
                HistoryChange historyChange{storedModels.getModelName(index), index, false, false, instanceTransformationVector[index]};

                modification.push_back(historyChange);

                switch(transformationData.transformationIdentifier)
                {
                    case DataStructures::TransformationIdentifier::RotationX:

                        instanceTransformationVector[index] = glm::rotate(instanceTransformationVector[index], glm::radians(transformationData.amount), glm::vec3{1.0f, 0.0f, 0.0f});

                        break;

                    case DataStructures::TransformationIdentifier::RotationY:

                        instanceTransformationVector[index] = glm::rotate(instanceTransformationVector[index], glm::radians(transformationData.amount), glm::vec3{0.0f, 1.0f, 0.0f});

                        break;

                    case DataStructures::TransformationIdentifier::RotationZ:

                        instanceTransformationVector[index] = glm::rotate(instanceTransformationVector[index], glm::radians(transformationData.amount), glm::vec3{0.0f, 0.0f, 1.0f});

                        break;

                    case DataStructures::TransformationIdentifier::ScaleX:

                        instanceTransformationVector[index] = glm::scale(instanceTransformationVector[index], glm::vec3{transformationData.amount, 1.0f, 1.0f});

                        break;

                    case DataStructures::TransformationIdentifier::ScaleY:

                        instanceTransformationVector[index] = glm::scale(instanceTransformationVector[index], glm::vec3{1.0f, transformationData.amount, 1.0f});

                        break;

                    case DataStructures::TransformationIdentifier::ScaleZ:

                        instanceTransformationVector[index] = glm::scale(instanceTransformationVector[index], glm::vec3{1.0f, 1.0f, transformationData.amount});

                        break;

                        // For some reason, after a rotation, the values returned by glm::translate are incorrect- the resulting
                        // translation is in a wrong dimension. Thus a manual edit of the transformation matrix is done.
                    case DataStructures::TransformationIdentifier::TranslationX:

                        instanceTransformationVector[index][3][0] += transformationData.amount;

                        break;

                    case DataStructures::TransformationIdentifier::TranslationY:

                        instanceTransformationVector[index][3][1] += transformationData.amount;

                        break;

                    case DataStructures::TransformationIdentifier::TranslationZ:

                        instanceTransformationVector[index][3][2] += transformationData.amount;

                        break;
                }
            }

            historyChanges.push_back(modification);

            transformationsVBO.uploadData(instanceTransformationVector);
        }

        void ModelVAO::undoUserAction()
        {
            // No user action therefore no action to take.
            if(historyChanges.empty())
            {
                return;
            }

            // All undo actions forget what instances the user has selected. This logically makes sense as the instance
            // the user selected may not longer exist after the undo action- and also prevents a crash that would occur
            // sometimes, which is a bonus.
            intersectionIndexes.clear();

            // To prevent potentially many similar function calls, the required operations are stored temporarily and then
            // the requested operation is done by passing all of the required data at once.
            QHash<QString, std::vector<glm::mat4x4>> addInstances;
            std::vector<unsigned int> removeIndexes;

            auto transformationVector = transformationsVBO.getHeldData();
            bool changedTransformations = false; // Keep track of whether to reupload the transformations.

            for(const auto &historyChange : historyChanges.back())
            {
                if(historyChange.addedMatrix) // Added a matrix, then the opposite is to delete it.
                {
                    removeIndexes.push_back(historyChange.index);
                }
                else if(historyChange.removedMatrix) // Removed a matrix, then the opposite is to add it back.
                {
                    if(!addInstances.contains(historyChange.model))
                    {
                        addInstances.insert(historyChange.model, std::vector<glm::mat4x4>{});
                    }

                    addInstances[historyChange.model].push_back(historyChange.previousMatrix);
                }
                else
                {
                    transformationVector[historyChange.index] = historyChange.previousMatrix; // Applied a transformation; the opposite is to use the
                    // matrix before the transformation was modified.

                    changedTransformations = true;
                }
            }

            // The user history was removing instances; time to add them back.
            if(!addInstances.isEmpty())
            {
                QHash<QString, std::vector<glm::mat4x4>>::const_iterator i = addInstances.constBegin();

                while (i != addInstances.constEnd())
                {
                    addModelInstances(i.key(), i.value(), true);

                    ++i;
                }
            }

            // The user history was adding instances; time to remove them.
            if(!removeIndexes.empty())
            {
                removeInstances(removeIndexes);
            }

            // The user applied transformations; time to remove those transformations.
            if(changedTransformations)
            {
                transformationsVBO.uploadData(transformationVector);
            }

            historyChanges.pop_back();
        }

        // Beginning of private functions

        void ModelVAO::removeInstances(std::vector<unsigned int> &indexes)
        {
            auto instanceTransformationVector = transformationsVBO.getHeldData();

            auto instanceColourVector = instanceColours.getHeldData();

            // Iterate over the selected instances, and remove them from being rendered.
            for(unsigned int i = 0; i < indexes.size(); ++i)
            {
                instanceTransformationVector.erase(instanceTransformationVector.begin() + indexes[i]);

                instanceColourVector.erase(instanceColourVector.begin() + indexes[i]);

                storedModels.removeInstance(indexes[i]);

                // Since the intersectionIndexes represent indexes into the related instances VBO, and the above code
                // erased an element from those VBOs, the next indexes need to have their indexes adjusted so that
                // they refer to their assigned element in the updated position in those VBOs.
                for(unsigned nextIndex = i + 1; nextIndex < indexes.size(); ++nextIndex)
                {
                    indexes[nextIndex] -= 1;
                }
            }

            indexes.clear();

            transformationsVBO.uploadData(instanceTransformationVector);

            instanceColours.uploadData(instanceColourVector);
        }

        void ModelVAO::uploadModel()
        {
            for(const auto &model : modelsToUpload)
            {
                for(const auto &mesh : model.getMeshes())
                {
                    const unsigned int numberVerticesHeld = verticesVBO.getHeldData().size();

                    std::vector<unsigned int> adjustedIndices = mesh.getIndices();

                    // The indices have to have an offset added to deal with the fact that there are other vertices in the
                    // vertices VBO, in order to reference vertices of its associated model.
                    for(unsigned int &indice : adjustedIndices)
                    {
                        indice += numberVerticesHeld;
                    }

                    verticesVBO.uploadDataAppend(mesh.getVertices());

                    indices.uploadDataAppend(adjustedIndices);

                    textureManager.uploadTexture(QString::fromStdString(mesh.getTextureLocation()));

                    std::vector<glm::vec2> adjustedTextureCoords = mesh.getTextureCoords();

                    // Adjust the texture coordinates for the fact that the associated mesh texture may have changed size
                    // to meet one of the available texture array sizes.
                    for(auto &textureCoord : adjustedTextureCoords)
                    {
                        textureCoord.x *= textureManager.getCompressFactor().xFactor;

                        textureCoord.y *= textureManager.getCompressFactor().yFactor;
                    }

                    textureCoordinates.uploadDataAppend(adjustedTextureCoords);
                }
            }

            modelsToUpload.clear();
        }
    }
}
