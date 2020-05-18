//
// Created by binybrion on 4/30/20.
//

#include "ShaderManager.h"

namespace Render
{
    namespace Shader
    {
        void ShaderManager::addShaderProgram(const QString &shaderProgramName, const QString &vertexShaderLocation, const QString &fragmentShaderLocation)
        {
            // Note that raw pointers are used as copying a QOpenGLShaderProgram is not allowed (meaning that pointers have to be used).
            // Using smart pointers (specifically unique_ptrs) lead to compilation issues that could not be resolved.
            shaderPrograms.insert(shaderProgramName, new QOpenGLShaderProgram);

            // Compile the passed in shaders

            if(!shaderPrograms[shaderProgramName]->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderLocation))
            {
                throw std::runtime_error{createShaderErrorMessage(shaderProgramName)};
            }

            if(!shaderPrograms[shaderProgramName]->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderLocation))
            {
                throw std::runtime_error{createShaderErrorMessage(shaderProgramName)};
            }

            // Link the shader program

            if(!shaderPrograms[shaderProgramName]->link())
            {
                throw std::runtime_error{createShaderErrorMessage(shaderProgramName)};
            }
        }

        QOpenGLShaderProgram &ShaderManager::getShaderProgram(const QString &shaderProgramName)
        {
            return *shaderPrograms[shaderProgramName];
        }

        ShaderManager::~ShaderManager()
        {
            for(auto i = shaderPrograms.begin(); i != shaderPrograms.end(); ++i)
            {
                // Make sure any resources associates with the shader program are are released well.
                i.value()->release();

                i.value()->removeAllShaders();

                // This will also delete the shader program OpenGL-resources (or at least, pretty sure it does according to QOpenGLShaderProgram Qt Doc).
                delete i.value();
            }
        }

        std::string ShaderManager::createShaderErrorMessage(const QString &qShaderProgramName) const
        {
            std::string errorMessage = "Error creating shader program. The following error message exists: \n\n";

            errorMessage += shaderPrograms[qShaderProgramName]->log().toStdString();

            return errorMessage;
        }
    }
}