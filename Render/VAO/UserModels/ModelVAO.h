//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_MODELVAO_H
#define VOXEL_L_SYSTEM_MODELVAO_H

#include <QtGui/QOpenGLFunctions_4_4_Core>
#include <Render/VBO/VBOWapper.h>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include "../../DataStructures/RecursionTree.h"
#include "../../DataStructures//StoredModels.h"

namespace ModelLoading
{
    class Model;
}

namespace Render
{
    namespace VAO
    {
        /**
         * Handles storing required data for user models as well as rendering (instances of) those models.
         */

        class ModelVAO : public QOpenGLFunctions_4_4_Core
        {
            public:

                /**
                 * Stores a model to be rendered for later. Until instances of the model are added, the model is not rendered.
                 *
                 * @param model containing the render data
                 */
                void addModel(const ::ModelLoading::Model &model);

                /**
                 * Add instances of a model to be rendered. The model must have been loaded first using addModel().
                 *
                 * @param modelFileName name of the file used to load the model
                 * @param recursionDepth the level of recursion the instance is a result of
                 * @param transformationMatrices the matrices representing instances of the model to render
                 */
                void addModelInstances(const QString &modelFileName, unsigned int recursionDepth, const std::vector<glm::mat4x4> &transformationMatrices);

                /**
                 * Checks for an intersection with the ray and an instance of a model. If there is one, that instance
                 * is highlighted a different colour.
                 *
                 * @param cameraPosition position of the camera in world space
                 * @param rayDirection direction of the ray from the camera position, in world space
                 */
                void checkRayIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection);

                /**
                 * Initializes all internal buffers for use for rendering.
                 */
                void initialize();

                /**
                 * Removes all instances of the passed in model at the specified recursion depth.
                 *
                 * @param modelFileName the name of the file used to loader the model
                 * @param recursionDepth recursion depth instances to remove
                 */
                void removeModelInstances(const QString &modelFileName, unsigned int recursionDepth);

                /**
                 * Renders the instances of models.
                 */
                void render();

                /**
                 * Any highlighted model instances are reset to the default colour.
                 */
                void resetIntersectionColours();

            private:
                void uploadModel();

                unsigned int vao;
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::vec3> verticesVBO;
                VBO::VBOWrapper<GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, unsigned int> indices;
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::mat4x4> transformationsVBO;
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::vec3> instanceColours;

                DataStructures::RecursionTree recursionTree;
                DataStructures::StoredModels storedModels;

                std::vector<unsigned int> intersectionIndexes;

                std::vector<::ModelLoading::Model> modelsToUpload;
        };
    }
}

#endif //VOXEL_L_SYSTEM_MODELVAO_H
