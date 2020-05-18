//
// Created by binybrion on 5/18/20.
//

#ifndef VOXEL_L_SYSTEM_TEXTUREMANAGER_H
#define VOXEL_L_SYSTEM_TEXTUREMANAGER_H

#include <QtGui/QOpenGLFunctions_4_4_Core>
#include "TextureBank.h"

namespace Render
{
    namespace Textures
    {
        class TextureManager : public QOpenGLFunctions_4_4_Core
        {
            public:

                TextureManager();
                void initialize();
                void uploadTexture(const QString &textureLocation);

            private:

                struct TextureRelation
                {
                    unsigned int textureVBO;
                    int textureID;
                    int textureCount = 0;
                    int maxNumberTextures;
                    int dimension;
                };

                void initializeTextureVBO(TextureRelation &textureRelations);
                void uploadTexture(TextureRelation &textureRelations, const QString &textureLocation, const QImage &image);

                TextureRelation textureRelations_512x512;
                TextureRelation textureRelations_1024x1024;
                TextureRelation textureRelations_2048x2048;

                TextureBank textureBank;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TEXTUREMANAGER_H
