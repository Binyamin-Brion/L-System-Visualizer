//
// Created by binybrion on 5/9/20.
//

#ifndef VOXEL_L_SYSTEM_PROJECTLOADER_H
#define VOXEL_L_SYSTEM_PROJECTLOADER_H

#include <QtCore/QString>
#include "L_System/DataStructures/Variable/Variable.h"
#include "L_System/DataStructures/Constants/Constant.h"
#include "L_System/DataStructures/Rules/Rule.h"
#include "ScriptInformation.h"
#include "ProjectDetails.h"

namespace ProjectSaverLoader
{
    /**
     * Loads a project that has been saved to a file. Afterwards, the project can be displayed in the GUI and the user
     * can continue where they last saved.
     */

    class ProjectLoader
    {
        public:

            /**
             * Loads the project stored in the given file.
             *
             * @param projectFileLocation location of the file containing the project to load
             * @return a copy of the project details containing the scripts stored in the project file
             */
            ProjectDetails loadProject(const QString &projectFileLocation);

        private:

            /**
             * Checks that the number of tokens on a given line matches one of the expected value.
             *
             * @param numberTokens on a given line in the project file
             * @param expectedNumberTokens vector of expected number of tokens. The numberTokens must match one of the numbers in this vector
             * @param fileLine line that is being checked
             * @throws runtime_exception if the number of tokens does not match any numbers in expectedNumberTokens
             */
            void checkExpectedNumberLineTokens(int numberTokens, std::vector<int> expectedNumberTokens, const QString &fileLine) const;

            /**
             * Clears the result of loading the previous project file.
             */
            void clearPreviousLoad();

            /**
             * Converts the given string to a number.
             *
             * @param stringToConvert to convert to a number
             * @return the float equivalent of the passed in string
             * @throws if the passed in string is not a valid floating point number
             */
            float convertNumber(const QString &stringToConvert) const;

            /**
             * Converts the given string to a stack operation.
             *
             * @param stringToConvert to convert to a stack operation
             * @return the stack-operation data structure equivalent of the passed in string
             */
            ::L_System::DataStructures::StackOperation convertStackOperation(const QString &stringToConvert) const;

            /**
             * Extracts the axiom from the given file line.
             *
             * @param fileLine from which to extract a script axiom
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadAxiom(const QString &fileLine);

            /**
             * Extracts the constant from the given file line.
             *
             * @param fileLine from which to extract a script constant
             * @throws runtime_exception if the file line is not of the expected format and either a number or a stack operation
             *                           could not be converted to its literal equivalent
             */
            void loadConstant(const QString &fileLine);

            /**
             * Extracts the bookmarked result from the given file line.
             *
             * @param fileLine from which to extract the bookmarked result
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadFavouriteScriptResult(const QString &fileLine);

            /**
             * Extracts the rule from the given file line.
             *
             * @param fileLine from which to extract a script rule
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadRule(const QString &fileLine);

            /**
             * Extracts the variable from the given file line.
             *
             * @param fileLine from which to extract a script variable
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadVariable(const QString &fileLine);

            ::L_System::DataStructures::Variable axiom;
            std::vector<::L_System::DataStructures::Constant> loadedConstants;
            std::vector<FavouriteResult> loadedFavouriteResults;
            std::vector<::L_System::DataStructures::Rule> loadedRules;
            std::vector<::L_System::DataStructures::Variable> loadedVariables;
    };
}

#endif //VOXEL_L_SYSTEM_PROJECTLOADER_H
