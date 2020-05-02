//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_VBOWAPPER_H
#define VOXEL_L_SYSTEM_VBOWAPPER_H

#include <QOpenGLFunctions_4_4_Core>

namespace Render
{
    namespace VBO
    {
        /**
         * A wrapper over a raw OpenGL VBO resource, providing an abstraction for creating and uploading data.
         *
         * Note: all of these functions must be called in the same thread as the OpenGL thread!
         *
         * @tparam bufferTarget to be used for the buffer
         * @tparam bufferUsage to be used for the buffer
         * @tparam T type of data the buffer will be storing
         */

        template<GLenum bufferTarget, GLenum bufferUsage, typename T>
        class VBOWrapper : public QOpenGLFunctions_4_4_Core
        {
            public:

                /**
                 * Binds the raw OpenGL resource.
                 */
                void bind()
                {
                    glBindBuffer(bufferTarget, vbo);
                }

                /**
                 * Creates the raw OpenGL resource.
                 */
                void initialize()
                {
                    if(!initializeOpenGLFunctions())
                    {
                        throw std::runtime_error{"Unable to initialize OpenGL functions at: " + std::string{__PRETTY_FUNCTION__}};
                    }

                    glGenBuffers(1, &vbo);
                }

                const std::vector<T>& getHeldData() const
                {
                    return heldData;
                }

                /**
                 * Uploads the passed in data, overwriting any previous data.
                 *
                 * @param data to be uploaded into the VBO.
                 */
                void uploadData(const std::vector<T> &data)
                {
                    heldData = data;

                    glBindBuffer(bufferTarget, vbo);
                    glBufferData(bufferTarget, data.size() * sizeof(T), data.data(), bufferUsage);
                }

                /**
                 * Uploads the passed in data, appending any previous data.
                 *
                 * @param data to be uploaded into the VBO.
                 */
                void uploadDataAppend(const std::vector<T> &data)
                {
                    heldData.insert(heldData.end(), data.begin(), data.end());

                    glBindBuffer(bufferTarget, vbo);
                    glBufferData(bufferTarget, heldData.size() * sizeof(T), heldData.data(), bufferUsage);
                }

            private:
                unsigned int vbo;
                std::vector<T> heldData;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VBOWAPPER_H
