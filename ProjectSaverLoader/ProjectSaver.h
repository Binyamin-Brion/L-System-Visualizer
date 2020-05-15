//
// Created by binybrion on 5/9/20.
//

#ifndef VOXEL_L_SYSTEM_PROJECTSAVER_H
#define VOXEL_L_SYSTEM_PROJECTSAVER_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <vec3.hpp>
#include "L_System/DataStructures/Constants/StackOperation.h"
#include "L_System/DataStructures/Variable/Variable.h"
#include "L_System/DataStructures/Rules/Rule.h"
#include "ProjectDetails.h"

namespace ProjectSaverLoader
{
    /**
     * Saves a project to a file on the disk, allowing it to be read back at a later date using the ProjectLoader.
     */

    class ProjectSaver
    {
        public:

            /**
             * Saves the project to the specified file location. If that file location does not exist, it will be created;
             * otherwise the previous contents will be overwritten.
             *
             * @param saveLocation the location to save the project to
             * @param projectDetails information of the project to write to the specified file
             */
            void saveProject(const QString &saveLocation, const ProjectDetails &projectDetails);

        private:

            /**
             * Converts the passed in StackOperation structure to the string equivalent.
             *
             * @param stackOperation to convert to a string
             * @return the string equivalent of the passed in stack operation
             */
            QString convertStackOperationString(::L_System::DataStructures::StackOperation stackOperation);

            /**
             * Converts the passed in vector structure to the string equivalent.
             *
             * @param vector to convert to a string
             * @return the string equivalent of the passed in vector
             */
            QString convertVectorString(const glm::vec3 &vector);

            /**
             * Writes the passed axiom to the project save file.
             *
             * @param axiom to save to disk
             */
            void saveAxiom(const ::L_System::DataStructures::Variable &axiom);

            /**
             * Writes the passed in constants to the project save file.
             *
             * @param constants to save to disk
             */
            void saveConstants(const std::vector<::L_System::DataStructures::Constant> &constants);

            /**
             * Writes the passed in name to the project save file.
             *
             * @param scriptName the name of the script being saved
             */
            void saveName(const QString &scriptName);

            /**
             * Writes the passed in rules to the project save file.
             *
             * @param rules to save to disk
             */
            void saveRules(const std::vector<::L_System::DataStructures::Rule> &rules);

            /**
             * Writes the output of a script to the project file.
             *
             * @param favouriteResults the list of bookmarked results to write to a project file
             */
            void saveRequestedScriptOutputs(const std::vector<FavouriteResult> &favouriteResults);

            /**
             * Writes the user added instance files of a bookmarked result to a project file.
             *
             * @param favouriteResults the list of bookmarked results containing user added instance models to save
             */
            void saveUserModelInstances(const std::vector<FavouriteResult> &favouriteResults);

            /**
             * Writes the passed in variables to the project save file.
             *
             * @param variables to save to disk
             */
            void saveVariables(const std::vector<::L_System::DataStructures::Variable> &variables);

            // Holds the file being written to.
            QFile file;
    };
}

#endif //VOXEL_L_SYSTEM_PROJECTSAVER_H
