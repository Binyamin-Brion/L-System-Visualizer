//
// Created by BinyBrion on 11/21/2019.
//

#ifndef MINECRAFT_MODEL_H
#define MINECRAFT_MODEL_H

class aiNode;
class aiScene;
class aiMesh;

#include <string>
#include <vector>
#include <QString>

#include "Mesh.h"
#include "Face.h"
#include "mat4x4.hpp"

namespace ModelLoading
{
    /**
     *  Loads a model from a file, and makes it available by returning the list of meshes that compose the model.
     */

    class Model
    {
        public:

            /**
             * Loads the model contained in the passed in file.
             *
             * @param fileLocation where the model information is held
             */
            explicit Model(const std::string &fileLocation);

            /**
             * Checks for an intersection between a ray with a known start position and this model. Every mesh is checked
             * by first converting it to world space using the passed in transformation matrix.
             *
             * @param cameraPosition position of the camera in world space
             * @param rayDirection direction of the camera in world space
             * @param transformationMatrix matrix to transform model into world space
             * @return true if there is an intersection
             */
            bool checkIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection, const glm::mat4 &transformationMatrix) const;

            /**
             * Get the individual meshes that compose the loaded model.
             *
             * @return vector of the model's meshes
             */
            const std::vector<Mesh>& getMeshes() const;

            /**
             * Get the name of the file used to load this model.
             *
             * @return name of file containing information used to load this model
             */
            const QString &getModelFileName() const;

        private:

            /**
             * Extracts all of the required rendering data for a mesh of the model.
             *
             * @param mesh pointer to the mesh resource
             * @param scene pointer to the scene resource
             */
            void processMesh(const aiMesh* const mesh, const aiScene* const scene);

            /**
             * Extracts all of the required rendering data for a mesh of the model.
             *
             * @param mesh pointer to the mesh resource
             * @param scene pointer to the scene resource
             */
            void processNode(const aiNode* const node, const aiScene* const scene);

            std::vector<Mesh> meshes;
            QString modelFileName;
    };
}

#endif //MINECRAFT_MODEL_H
