//
// Created by binybrion on 5/9/20.
//

#ifndef VOXEL_L_SYSTEM_PROJECTDETAILS_H
#define VOXEL_L_SYSTEM_PROJECTDETAILS_H

#include "ScriptInformation.h"

namespace ProjectSaverLoader
{
    /**
     * Holds the state of a script that can later be used to save the script's state to a file, as well as the result
     * of reading a script from a file.
     */

    class ProjectDetails
    {
        public:

            /**
             * Adds a script information to the scripts held by this class.
             *
             * @param saveAxiom the script's axiom
             * @param saveConstants the script's constants
             * @param saveRules the script's rules
             * @param saveVariables the script's variables
             */
            void addScriptInformation(const ::L_System::DataStructures::Variable &saveAxiom,
                                             const std::vector<::L_System::DataStructures::Constant> &saveConstants,
                                             const std::vector<::L_System::DataStructures::Rule> &saveRules,
                                             const std::vector<::L_System::DataStructures::Variable> &saveVariables);

            /**
             * Get the scripts that have been uploaded to this class.
             *
             * @return vector of scripts that have been added to this class
             */
            const std::vector<ScriptInformation>& getScripts() const;

        private:

            std::vector<ScriptInformation> scripts;
    };
}

#endif //VOXEL_L_SYSTEM_PROJECTDETAILS_H
