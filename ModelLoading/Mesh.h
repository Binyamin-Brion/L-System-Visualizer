//
// Created by BinyBrion on 11/21/2019.
//

#ifndef MINECRAFT_MESH_H
#define MINECRAFT_MESH_H

#include <vector>
#include <vec2.hpp>
#include <vec3.hpp>
#include <string>
#include "Face.h"

namespace ModelLoading
{
    /**
     *  Abstracts the pieces of data that are needed to represent a component to render a component within a model.
     *  It is a "container" that aggregates the necessary rendering data for a mesh, and contains an AABB around the
     *  vertices that make this mesh.
     */

    class Mesh
    {
        public:

            /**
             * Initializes the mesh with the necessary information used to for rendering.
             *
             * @param vertices that the mesh is composed of in model space
             * @param indices used to specify the ordering of vertices when rendering
             * @param normals associated with each vertex
             * @param textureCoords that each vertex has
             * @param textureLocation location to the model on the file system from which the mesh data was loaded
             */
            Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices,
                 std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, std::string textureLocation);


            const std::vector<Face>& getFaces() const;

            /**
             * Get the indices that make up this mesh.
             *
             * @return reference to the indices vector
             */
            const std::vector<unsigned int>& getIndices() const;

            /**
             * Get the normals that are associated with each mesh vertex.
             *
             * @return reference to the normal vector
             */
            const std::vector<glm::vec3>& getNormals() const;

            /**
             * Get the texture coordinates associated with each mesh vertex.
             *
             * @return reference to the texture coordinates vector
             */
            const std::vector<glm::vec2>& getTextureCoords() const;

            /**
             * Get the texture location that the mesh uses.
             *
             * @return the location of the texture on the file system used by the mesh
             */
            const std::string& getTextureLocation() const;

            /**
             * Get the vertices that make up this mesh.
             *
             * @return reference to the vertices vector
             */
            const std::vector<glm::vec3>& getVertices() const;

        private:

            /**
             * Calculates the faces that make up a mesh using the vertices and indices used for rendering.
             */
            void findFaces();

            std::vector<glm::vec3> vertices;
            std::vector<unsigned int> indices;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> textureCoords;
            std::string textureLocation;
            std::vector<Face> faces;
    };
}

#endif //MINECRAFT_MESH_H
