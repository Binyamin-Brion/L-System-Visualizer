//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_MODELVAO_H
#define VOXEL_L_SYSTEM_MODELVAO_H

#include <QtGui/QOpenGLFunctions_4_2_Core>
#include <Render/VBO/VBOWapper.h>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <ProjectSaverLoader/UserDefinedInstances.h>
#include "../../DataStructures//StoredModels.h"
#include "../../DataStructures/TransformationData.h"
#include "../../Textures/TextureManager.h"

namespace ModelLoading
{
    class Model;
}

namespace ProjectSaverLoader
{
    class ProjectDetails;
}

namespace Render
{
    namespace Shader
    {
        class ShaderManager;
    }

    namespace VAO
    {
        /**
         * Handles storing required data for user models as well as rendering (instances of) those models.
         */

        class ModelVAO : public QOpenGLFunctions_4_2_Core
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
                 * @param transformationMatrices the matrices representing instances of the model to render
                 */
                void addModelInstances(const QString &modelFileName, const std::vector<glm::mat4x4> &transformationMatrices, bool userAddedIndex = false);

                /**
                 * Adds an instance of the given model to the render. The instance will be centred around the origin.
                 *
                 * @param modelFileName name of the instance to add
                 */
                void addUserRequestedModelInstance(const QString &modelFileName);

                /**
                 * Renders all of the model instances specified in the parameter. This is called when rerendering an interpretation result.
                 *
                 * Any previous user action history is removed.
                 *
                 * @param modelInstances the list of instances for every model that needs instance(s) to be rendered
                 */
                void addUserRequestedModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &modelInstances);

                /**
                 * Checks for an intersection with the ray and an instance of a model. If there is one, that instance
                 * is highlighted a different colour.
                 *
                 * @param cameraPosition position of the camera in world space
                 * @param rayDirection direction of the ray from the camera position, in world space
                 */
                void checkRayIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection, bool appendIntersections);

                /**
                 * Removes all OpenGL rendering data. Afterwards, nothing will be rendered.
                 */
                void deleteOpenGLResources();

                /**
                 * Deletes the selected instances. Afterwards they will no longer be rendered.
                 */
                void deleteSelectedInstances();

                /**
                * Exports the currently rendered model instances to the given location.
                *
                * @param exportLocation location of the file to export to
                */
                void exportCurrentRender(const QString &exportLocation);

                /**
                 * Get the user defined instances of loaded models currently being rendered.
                 *
                 * @return list of currently rendered user-defined models
                 */
                std::vector<::ProjectSaverLoader::UserDefinedInstances> getUserDefinedInstances() const;

                /**
                 * Initializes all internal buffers for use for rendering.
                 */
                void initialize();

                /**
                 * Removes all instances of the passed in model, both logically and the data used for rendering.
                 *
                 * @param modelFileName the name of the file used to loader the model
                 */
                void removeModelInstances(const QString &modelFileName);

                /**
                 * Renders the instances of models.
                 */
                void render(Shader::ShaderManager &shaderManager);

                /**
                 * Applies the given transformation to all of the rendered user added instances.
                 *
                 * This call is forwarded to the ModelVAO.
                 *
                 * @param transformationData transformation to apply to selected instances
                 */
                void transformSelectedModels(const DataStructures::TransformationData &transformationData);

                /**
                 * Undoes the most recent user action done in the render scene, if there is one.
                 */
                void undoUserAction();

            private:

                /**
                 * Removes all instances associated with the indexes held in the passed in parameter.
                 *
                 * The passed in indexes vector will be cleared at the end of this function, to signify that all instances
                 * have been deleted.
                 *
                 * @param indexes specifying what instances to remove
                 */
                void removeInstances(std::vector<unsigned int> &indexes);

                /**
                 * Uploads the deferred models into vRam.
                 */
                void uploadModel();

                unsigned int vao;
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::vec3> verticesVBO;
                VBO::VBOWrapper<GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, unsigned int> indices;

                // These hold the instance transformation matrices.
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::mat4x4> transformationsVBO;

                // Do NOT use std::vector<bool> here! It gives compilation issues, and has weird behaviour in general.
                // (Even though the unsigned char holds only true or false)
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, unsigned char> instanceColours;
                VBO::VBOWrapper<GL_ARRAY_BUFFER, GL_STATIC_DRAW, glm::vec2> textureCoordinates;

                DataStructures::StoredModels storedModels;

                std::vector<unsigned int> intersectionIndexes;

                std::vector<::ModelLoading::Model> modelsToUpload;

                struct HistoryChange
                {
                    QString model;
                    unsigned int index;
                    bool addedMatrix;
                    bool removedMatrix;
                    glm::mat4x4 previousMatrix;
                };

                using Modification = std::vector<HistoryChange>;

                std::vector<Modification> historyChanges;

                Textures::TextureManager textureManager;
        };
    }
}

#endif //VOXEL_L_SYSTEM_MODELVAO_H
