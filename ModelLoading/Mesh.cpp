//
// Created by BinyBrion on 11/21/2019.
//

#include "Mesh.h"
#include <algorithm>
#include <stdexcept>

namespace ModelLoading
{
    Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices,
               std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, std::string textureLocation)
               :
                   vertices{std::move(vertices)},
                   indices{std::move(indices)},
                   normals{std::move(normals)},
                   textureCoords{std::move(textureCoords)},
                   textureLocation{std::move(textureLocation)}
    {
        findFaces();
    }

    const std::vector<Face>& Mesh::getFaces() const
    {
        return faces;
    }

    const std::vector<unsigned int>& Mesh::getIndices() const
    {
        return indices;
    }

    const std::vector<glm::vec3>& Mesh::getNormals() const
    {
        return normals;
    }

    const std::vector<glm::vec2>& Mesh::getTextureCoords() const
    {
        return textureCoords;
    }

    const std::string& Mesh::getTextureLocation() const
    {
        return textureLocation;
    }

    const std::vector<glm::vec3>& Mesh::getVertices() const
    {
        return vertices;
    }

    void Mesh::findFaces()
    {
        // The mesh was triangulated when read from the resource file. Thus every face has three vertices, indexed into
        // the vertex array using three indices. As a result the number of indices must be a number divisible by 3.

        if(indices.size() % 3 != 0)
        {
            throw std::runtime_error{"The loaded model contains a mesh with an unexpected format. Face selection will not work!"};
        }

        for(unsigned int i = 0; i < indices.size(); i += 3)
        {
            faces.emplace_back(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
        }
    }
}