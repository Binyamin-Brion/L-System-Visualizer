//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_SHADERMANAGER_H
#define VOXEL_L_SYSTEM_SHADERMANAGER_H

#include <QOpenGLShaderProgram>
#include <QHash>

namespace Render
{
    namespace Shader
    {
        /**
         * Manages all of the shader programs used in this program. Provides the ability to create them as well as
         * retrieve them in order to call functions on them directly.
         */

        class ShaderManager
        {
            public:

                /**
                 * Creates a shader program using the shader files provided and stores it by the name provided.
                 *
                 * @param shaderProgramName name of the shader program being created. Will be referred later by this name.
                 * @param vertexShaderLocation location of the file containing the vertex shader
                 * @param fragmentShaderLocation location of the file containing the fragment shader
                 */
                void addShaderProgram(const QString &shaderProgramName, const QString &vertexShaderLocation, const QString &fragmentShaderLocation);

                /**
                 * Gets a reference to the shader program referred to by the passed in name.
                 *
                 * @param shaderProgramName name of the shader program used when creating the program.
                 * @return reference to the shader program associated with the passed in name
                 */
                QOpenGLShaderProgram& getShaderProgram(const QString &shaderProgramName);

                /**
                 * Deletes all shader programs created.
                 */
                ~ShaderManager();

            private:

                /**
                 * Creates an error message that can be used to throw an exception.
                 *
                 * @param qShaderProgramName name of the shader program that encountered an error.
                 * @return string containing the logs of the passed in shader program.
                 */
                std::string createShaderErrorMessage(const QString &qShaderProgramName) const;

                QHash<QString, QOpenGLShaderProgram*> shaderPrograms;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SHADERMANAGER_H
