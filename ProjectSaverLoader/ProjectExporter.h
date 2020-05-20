//
// Created by binybrion on 5/20/20.
//

#ifndef VOXEL_L_SYSTEM_PROJECTEXPORTER_H
#define VOXEL_L_SYSTEM_PROJECTEXPORTER_H

#include <QtCore/QString>
#include <mat4x4.hpp>
#include <vector>
#include <QFile>

namespace Render
{
    namespace DataStructures
    {
        class StoredModels;
    }
}

namespace ProjectSaverLoader
{
    /**
     * Writes the data stored in the current render to a file.
     */

    class ProjectExporter
    {
        public:

            /**
             * Creates a file at the given location passed as an argument, and writes the data passed in to that file.
             *
             * @param exportLocation where the export file should be created
             * @param storedModels the models that are being stored in vRam
             * @param instanceMatrices the instance matrices that specify model instances in the current render
             */
            void exportModelInstances(const QString &exportLocation,
                                      const ::Render::DataStructures::StoredModels &storedModels,
                                      const std::vector<glm::mat4x4> &instanceMatrices);

        private:

            /**
             * Writes a matrix to the created export file.
             *
             * @param matrix to write to the export file
             */
            void writeInstanceMatrix(const glm::mat4x4 &matrix);

            /**
             * Writes a model name to the the created export file
             *
             * @param modelName to write to the export file
             */
            void writeModelName(const QString &modelName);

            QFile exportFile;
    };
}

#endif //VOXEL_L_SYSTEM_PROJECTEXPORTER_H
