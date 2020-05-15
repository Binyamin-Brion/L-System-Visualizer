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
             * Checks section is encountered when reading the project file, and updating the readingXXX flags so that the
             * next lines are processed according to what information is stored in those next line(s).
             *
             * @param currentLine
             */
            void checkSectionTag(const QString &currentLine);

            /**
             * Chooses which one of the loadXXX function to call based off of the readingXXX flags.
             *
             * @param currentLine from which to extract script data
             */
            void chooseLoadFunction(const QString &currentLine);

            /**
             * Clears the result of loading the previous project file.
             */
            void clearPreviousLoad();

            /**
             * Converts the given string to a float.
             *
             * @param stringToConvert to convert to a float
             * @return the float equivalent of the passed in string
             * @throws if the passed in string is not a valid floating point number
             */
            float convertNumberFloat(const QString &stringToConvert) const;

            /**
            * Converts the given string to an unsigned int.
            *
            * @param stringToConvert to convert to a unsigned int
            * @return the float equivalent of the passed in string
            * @throws if the passed in string is not a valid floating point number
            */
            unsigned int convertNumberUnsigned(const QString &stringToConvert) const;

            /**
             * Converts the given string to a stack operation.
             *
             * @param stringToConvert to convert to a stack operation
             * @return the stack-operation data structure equivalent of the passed in string
             */
            ::L_System::DataStructures::StackOperation convertStackOperation(const QString &stringToConvert) const;

            /**
             * Updates the readingXXX flags so that the script name is read from the project file.
             */
            void handleScriptNameFlags();

            /**
             * Updates the readingXXX flags so that the variables are read from the project file.
             */
            void handleVariableFlags();

            /**
             * Updates the readingXXX flags so that the script axiom is read from the project file.
             */
            void handleAxiomFlags();

            /**
             * Updates the readingXXX flags so that the script constants is read from the project file.
             */
            void handleConstantFlags();

            /**
             * Updates the readingXXX flags so that the script rules are read from the project file.
             */
            void handleRuleFlags();

            /**
             * Updates the readingXXX flags so that the script bookmarked (favourite) results are read from the project file.
             */
            void handleFavouriteResultFlags();

            /**
             * Updates the readingXXX flags so that the script instance matrices added by the user are read from the project file.
             */
            void handleUserInstancesFlags();

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
             * Extracts the name of the script from the given file line.
             *
             * @param fileLine from which to extract the script name
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadScriptName(const QString &fileLine);

            /**
             * Extracts the rule from the given file line.
             *
             * @param fileLine from which to extract a script rule
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadRule(const QString &fileLine);

            /**
             * Extracts the instance associated with a favourite result from the given line.
             *
             * @param fileLine from which to extract an instance matrix
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadUserAddedInstances(const QString &fileLine);

            /**
             * Extracts the variable from the given file line.
             *
             * @param fileLine from which to extract a script variable
             * @throws runtime_exception if the file line is not of the expected format
             */
            void loadVariable(const QString &fileLine);

            /**
             * Resets the readingXXX flags to the default state.
             */
            void resetFlags();

            bool readScriptName = false;

            ::L_System::DataStructures::Variable axiom;
            std::vector<::L_System::DataStructures::Constant> loadedConstants;
            std::vector<FavouriteResult> loadedFavouriteResults;
            QString loadedScriptName;
            std::vector<::L_System::DataStructures::Rule> loadedRules;
            std::vector<UserDefinedInstances> loadedUserDefinedInstances;
            std::vector<::L_System::DataStructures::Variable> loadedVariables;

            // Controls how to interpret the current line based off of the previous section tag (Variables:, Constants:, etc).
            bool modifiedSectionFlag = false; // Know when to continue to next loop iteration after reading a section (Variable:, Constants:, etc) tag.

            // Note: the axiom and name are contained on the same line as the section tag, and since there is only one of each
            // meaning the results for them are not stored in a vector, a variable is needed to keep track of whether or not they were read.

            bool readAxiom = false;
            bool readingConstants = false;
            bool readingRules = false;
            bool readingVariables = false;
            bool readingFavouriteResults = false;
            bool readingUserDefinedInstances = false;
    };
}

#endif //VOXEL_L_SYSTEM_PROJECTLOADER_H
