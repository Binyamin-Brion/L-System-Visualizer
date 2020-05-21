//
// Created by binybrion on 5/18/20.
//

#include <QtGui/QImage>
#include "TextureManager.h"
#include <QPixmap>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

namespace Render
{
    namespace Textures
    {
        TextureManager::TextureManager()
        {
            // These are random values; they were assumed to be adequate for the given projects intended to be created with
            // this program while not consuming too much vRam. This may have to be adjusted based off of the project.
            textureRelations_512x512.maxNumberTextures = 50;
            textureRelations_1024x1024.maxNumberTextures = 25;
            textureRelations_2048x2048.maxNumberTextures = 12;

            textureRelations_512x512.textureID = 0;
            textureRelations_1024x1024.textureID = 1;
            textureRelations_2048x2048.textureID = 2;

            textureRelations_512x512.dimension = 512;
            textureRelations_1024x1024.dimension = 1024;
            textureRelations_2048x2048.dimension = 2048;
        }

        void TextureManager::deleteOpenGLResources()
        {
            // Ensure that no texture is being bounded, to ensure that the delete texture calls actually delete the texture resource.
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

            glDeleteTextures(1, &textureRelations_512x512.textureVBO);
            glDeleteTextures(1, &textureRelations_1024x1024.textureVBO);
            glDeleteTextures(1, &textureRelations_2048x2048.textureVBO);
        }

        CompressFactor TextureManager::getCompressFactor() const
        {
            return compressFactor;
        }

        const TextureBank &TextureManager::getTextureBank() const
        {
            return textureBank;
        }

        QImage TextureManager::fillUnusedArea(const QImage &image, int desiredLength)
        {
            // Create a texture of the required size and format.
            QImage filledImage = QPixmap{desiredLength, desiredLength}.toImage();

            filledImage = filledImage.convertToFormat(image.format());

            // Embedded the pixels of the passed in image in the top-left corner of the QImage; if the image does not
            // extend past a point in the bigger created image, then put a black pixel.

            /*
             *  Eg:
             *
             *  |------------------|
             *  |   Inner Image |  |
             *  |----------------  |
             *  |                  |
             *  |  Outer Image     |
             *  -------------------
             */
            for(int x = 0; x < desiredLength; ++x)
            {
                for(int y = 0; y < desiredLength; ++y)
                {
                    if(x >= image.width() || y >= image.height())
                    {
                        filledImage.setPixelColor(x, y, QColor{0, 0, 0, 0});
                    }
                    else
                    {
                        filledImage.setPixelColor(x, y, image.pixel(x, y));
                    }
                }
            }

             compressFactor = CompressFactor{static_cast<float>(image.width()) / static_cast<float>(desiredLength),
                                            static_cast<float>(image.height()) / static_cast<float>(desiredLength)};

            return filledImage;
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

            // No texture arrays created hold a texture larger than 2048 x 2048.
            if(image.width() > 2048 || image.height() > 2048)
            {
                throw std::runtime_error{"Only textures up to 2048 x 2048 are supported!"};
            }

            // Ensure that all textures passed in are of the same format specified for the texture arrays. RGBA was chosen
            // due to the fact that it satisfies the needs of almost everyone.
            image = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);

            // If a texture was uploaded in a uploadTexture(...) call, then the next call to uploadTexture() will return
            // as the texture location will exist in the texture bank. If by all three function calls, no entry in the texture
            // bank exists for the texture location, then the upload fails and an exception is thrown.
            uploadTexture(textureRelations_512x512, textureLocation, image);
            uploadTexture(textureRelations_1024x1024, textureLocation, image);
            uploadTexture(textureRelations_2048x2048, textureLocation, image);

            if(!textureBank.containsTexture(textureLocation))
            {
                throw std::runtime_error{"Unable to find space to load the texture: " + textureLocation.toStdString() + "."
                                                                                                                        "\nMax number of supported textures:\n\n"
                                                                                                                        "512x512: " + std::to_string(textureRelations_512x512.maxNumberTextures) + "\n" +
                                                                                                                        "1024x1024: " + std::to_string(textureRelations_1024x1024.maxNumberTextures) + "\n" +
                                                                                                                        "2048x2048: " + std::to_string(textureRelations_2048x2048.maxNumberTextures)};
            }
        }

        // Beginning of private functions

        void TextureManager::initializeTextureVBO(TextureManager::TextureRelation &textureRelations)
        {
            const unsigned int mipMapLevel = 1;

            glGenTextures(1, &textureRelations.textureVBO);
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureRelations.textureVBO);

            glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipMapLevel, GL_RGBA8, textureRelations.dimension, textureRelations.dimension, textureRelations.maxNumberTextures);

            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        void TextureManager::uploadTexture(TextureRelation &textureRelations, const QString &textureLocation, const QImage &image)
        {
            // Texture too big for this texture array.
            if(image.width() > textureRelations.dimension || image.width() > textureRelations.dimension)
            {
                return;
            }

            // No more space in the texture array.
            if(textureRelations.textureCount == textureRelations.maxNumberTextures)
            {
                return;
            }

            // Create an adequately sized texture to uploaded, if the given texture is not of the correct size.
            // Do this now so that an updated CompressionFactor is created for the caller of this function, that will be
            // correct even if the texture has already been uploaded into vRam. This is required because the face of a model
            // using a texture may be different from each other.
            QImage imageToUpload = fillUnusedArea(image, textureRelations.dimension);

            // Texture already loaded; don't load it again. Waste of vRam.
            if(textureBank.containsTexture(textureLocation))
            {
                return;
            }

            glActiveTexture(GL_TEXTURE0 + textureRelations.textureID);
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureRelations.textureVBO);
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0,  textureRelations.textureCount, textureRelations.dimension, textureRelations.dimension, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageToUpload.bits());

            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

            // Make note of the fact that the texture was uploaded.
            textureBank.insertTexture(textureLocation, {textureRelations.textureID, textureRelations.textureCount});

            textureRelations.textureCount += 1;
        }
    }
}