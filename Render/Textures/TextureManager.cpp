//
// Created by binybrion on 5/18/20.
//

#include <QtGui/QImage>
#include "TextureManager.h"

namespace Render
{
    namespace Textures
    {
        TextureManager::TextureManager()
        {
            textureRelations_512x512.maxNumberTextures = 25;
            textureRelations_1024x1024.maxNumberTextures = 16;
            textureRelations_2048x2048.maxNumberTextures = 8;

            textureRelations_512x512.textureID = 0;
            textureRelations_1024x1024.textureID = 0;
            textureRelations_2048x2048.textureID = 0;

            textureRelations_512x512.dimension = 512;
            textureRelations_1024x1024.dimension = 1024;
            textureRelations_2048x2048.dimension = 2048;
        }

        void TextureManager::initialize()
        {
            if(!initializeOpenGLFunctions())
            {
                throw std::runtime_error{"Failed to initialize GL functions at : " + std::string{__PRETTY_FUNCTION__}};
            }

            glActiveTexture(GL_TEXTURE0);
            initializeTextureVBO(textureRelations_512x512);

            glActiveTexture(GL_TEXTURE1);
            initializeTextureVBO(textureRelations_1024x1024);

            glActiveTexture(GL_TEXTURE2);
            initializeTextureVBO(textureRelations_2048x2048);
        }

        void TextureManager::uploadTexture(const QString &textureLocation)
        {
            if(textureLocation.isEmpty() || textureBank.containsTexture(textureLocation))
            {
                return;
            }

            QImage image;

            if(!image.load(textureLocation))
            {
                throw std::runtime_error{"Unable to load the texture: " + textureLocation.toStdString()};
            }

            image = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);

            uploadTexture(textureRelations_512x512, textureLocation, image);
            uploadTexture(textureRelations_1024x1024, textureLocation, image);
            uploadTexture(textureRelations_2048x2048, textureLocation, image);

            if(!textureBank.containsTexture(textureLocation))
            {
                throw std::runtime_error{"Unable to find space to load the texture: " + textureLocation.toStdString()};
            }
        }

        // Beginning of private functions

        void TextureManager::initializeTextureVBO(TextureManager::TextureRelation &textureRelations)
        {
            const unsigned int mipMapLevel = 1;

            glGenTextures(1, &textureRelations.textureVBO);
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureRelations.textureVBO);

            glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipMapLevel, GL_RGBA8, textureRelations.dimension, textureRelations.dimension, textureRelations.maxNumberTextures);

            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        }

        void TextureManager::uploadTexture(TextureRelation &textureRelations, const QString &textureLocation, const QImage &image)
        {
            if(textureBank.containsTexture(textureLocation))
            {
                return;
            }

            if(image.width() > textureRelations.dimension || image.width() > textureRelations.dimension)
            {
                return;
            }

            if(textureRelations.textureCount == textureRelations.maxNumberTextures)
            {
                return;
            }

            glBindTexture(GL_TEXTURE_2D_ARRAY, textureRelations.textureVBO);
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureRelations.textureCount, textureRelations.dimension, textureRelations.dimension, 1, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

            textureBank.insertTexture(textureLocation, {textureRelations.textureID, textureRelations.textureCount});

            textureRelations.textureCount += 1;
        }
    }
}