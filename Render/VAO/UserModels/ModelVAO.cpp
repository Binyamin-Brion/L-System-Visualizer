//
// Created by binybrion on 4/30/20.
//

#include "ModelVAO.h"
#include "ModelLoading/Model.h"

namespace Render
{
    namespace VAO
    {
        void ModelVAO::addModel(const ::ModelLoading::Model &model)
        {
            glBindVertexArray(vao);

            storedModels.addModel(model);

            modelsToUpload.push_back(model);
        }

        void ModelVAO::addModelInstances(const QString &modelFileName, unsigned int recursionDepth, const std::vector<glm::mat4x4> &transformationMatrices)
        {
            DataStructures::RecursionModelRange model(modelFileName, storedModels.getModelRange(modelFileName).getInstanceMatrixBegin() + storedModels.getModelRange(modelFileName).getInstanceMatrixCount(), transformationMatrices.size());

            recursionTree.addModelRecursiveResult(recursionDepth, model);

            // Add the transformation matrices to the required location in the instance matrices vector. This has to be done so that the
            // instance rendering will render only those instances associated with a particular model.
            auto instanceTransformationVector = transformationsVBO.getHeldData();

            instanceTransformationVector.insert(instanceTransformationVector.begin() + storedModels.getModelRange(modelFileName).getInstanceMatrixBegin() + storedModels.getModelRange(modelFileName).getInstanceMatrixCount(), transformationMatrices.begin(), transformationMatrices.end());

            transformationsVBO.uploadData(instanceTransformationVector);

            // Add instance colours to the instances of the model.
            std::vector<glm::vec3> instanceColourVector;

            for(unsigned int i = 0; i < transformationMatrices.size(); ++i)
            {
                instanceColourVector.emplace_back(0.5, 0.5, 0.5);
            }

            instanceColours.uploadDataAppend(instanceColourVector);

            storedModels.addModelInstances(modelFileName, transformationMatrices.size());
        }

        void ModelVAO::checkRayIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection)
        {
            for(unsigned int i = 0; i < transformationsVBO.getHeldData().size(); ++i)
            {
                if(storedModels.checkIntersection(cameraPosition, rayDirection, transformationsVBO.getHeldData()[i], i))
                {
                    // Only update the colour of the instance if it is not already coloured.
                    auto intersectionIndex = std::find(intersectionIndexes.begin(), intersectionIndexes.end(), i);

                    if(intersectionIndex == intersectionIndexes.end())
                    {
                        auto instanceColourVector = instanceColours.getHeldData();

                        instanceColourVector[i] = glm::vec3{0.6f, 0.5, 0.5};

                        instanceColours.uploadData(instanceColourVector);

                        intersectionIndexes.push_back(i);
                    }
                }
                else
                {
                    // Only update the instance to the default colour if it is highlighted already.
                    auto intersectionIndex = std::find(intersectionIndexes.begin(), intersectionIndexes.end(), i);

                    if(intersectionIndex != intersectionIndexes.end())
                    {
                        auto instanceColourVector = instanceColours.getHeldData();

                        instanceColourVector[i] = glm::vec3{0.5f, 0.5, 0.5};

                        instanceColours.uploadData(instanceColourVector);

                        intersectionIndexes.erase(intersectionIndex);
                    }
                }
            }
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

            glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
            glVertexAttribDivisor(5, 1);
            glEnableVertexAttribArray(5);
        }

        void ModelVAO::removeModelInstances(const QString &modelFileName, unsigned int recursionDepth)
        {
            auto removeResult = recursionTree.removeModelRecursiveResult(recursionDepth, modelFileName);

            if(removeResult.entryRemoved) // There is the requested model at the specified recursion level
            {
                auto instanceMatrices = transformationsVBO.getHeldData();

                // Remove instances from being rendered
                instanceMatrices.erase(instanceMatrices.begin() + removeResult.instanceMatrixBeginIndex, instanceMatrices.begin() + removeResult.instanceMatrixBeginIndex + removeResult.instanceMatrixCount);

                transformationsVBO.uploadData(instanceMatrices);

                storedModels.removeModelInstances(modelFileName, removeResult.instanceMatrixBeginIndex, removeResult.instanceMatrixCount);
            }
        }

        void ModelVAO::render()
        {
            glBindVertexArray(vao);

            uploadModel();

            // Instance render the required number of instances for each model.
            for(const auto &i : storedModels.getModelRanges())
            {
              //  printf("%d, %d \n", i.getInstanceMatrixCount(), i.getInstanceMatrixBegin());

                glDrawElementsInstancedBaseInstance(GL_TRIANGLES, i.getIndiceCount(), GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * i.getIndiceBegin()),
                                                    i.getInstanceMatrixCount(), i.getInstanceMatrixBegin());
            }
        }

        void ModelVAO::resetIntersectionColours()
        {
            // If no instances are highlighted, then return immediately. Not doing so is a waste of time.
            if(intersectionIndexes.empty())
            {
                return;
            }

            auto instanceColourVector = instanceColours.getHeldData();

            for(const auto &i : intersectionIndexes)
            {
                instanceColourVector[i] = glm::vec3{0.5f, 0.5f, 0.5f};
            }

            instanceColours.uploadData(instanceColourVector);

            intersectionIndexes.clear();
        }

        void ModelVAO::uploadModel()
        {
            for(const auto &model : modelsToUpload)
            {
                for(const auto &mesh : model.getMeshes())
                {
                    std::vector<unsigned int> adjustedIndices = mesh.getIndices();

                    // The indices have to have an offset added to deal with the fact that there are other vertices in the
                    // vertices VBO, in order to reference vertices of its associated model.
                    for(unsigned int &indice : adjustedIndices)
                    {
                        indice += verticesVBO.getHeldData().size();
                    }

                    verticesVBO.uploadDataAppend(mesh.getVertices());

                    indices.uploadDataAppend(adjustedIndices);
                }
            }

            modelsToUpload.clear();
        }
    }
}
