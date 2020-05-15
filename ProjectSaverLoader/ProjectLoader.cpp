//
// Created by binybrion on 5/9/20.
//

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "ProjectLoader.h"
#include "HelperFunctions/RuleLoading.h"
#include "ProjectDetails.h"

namespace ProjectSaverLoader
{
    ProjectDetails ProjectLoader::loadProject(const QString &projectFileLocation)
    {
        // Holds the details of the project that is being loaded.
        ProjectDetails projectDetails;

        // Ensure that any previous load does not affect this one.
        resetFlags();
        clearPreviousLoad();

        QFile projectFile{projectFileLocation};

        if(!projectFile.open(QFile::ReadOnly | QFile::Text))
        {
            throw std::runtime_error{"Unable to open the file: " + projectFileLocation.toStdString()};
        }

        QTextStream inputStream{&projectFile};

        while(!inputStream.atEnd())
        {
            QString currentLine = inputStream.readLine();

            // Empty lines are only to make the project file readable; they have no meaning.
            if(currentLine.isEmpty())
            {
                continue;
            }

            checkSectionTag(currentLine);

            // If a section tag was read, then the contents for the section are on the next line(s). Don't interpret
            // the current line as having the section information.
            if(modifiedSectionFlag)
            {
                modifiedSectionFlag = false;

                continue;
            }

            // End of the script; all information has been read and it is time to create a script information struct from
            // that information.
            if(currentLine.contains("END SCRIPT"))
            {
                if(!readScriptName)
                {
                    throw std::runtime_error{"There was a script encountered without a script name!\n"};
                }

                if(!readAxiom)
                {
                    throw std::runtime_error{qPrintable("The script: " + loadedScriptName + " does not have an axiom!\n")};
                }

                projectDetails.addScriptInformation(loadedScriptName, axiom, loadedConstants, loadedRules, loadedVariables, loadedFavouriteResults);
              //  projectDetails.addUserDefinedInstances(loadedScriptName, loadedUserDefinedInstances);

                // Reset the state of how to interpret each line.
                resetFlags();
                clearPreviousLoad();
            }

            chooseLoadFunction(currentLine);
        }

        return projectDetails;
    }

    // Beginning of private functions

    void ProjectLoader::clearPreviousLoad()
    {
        loadedConstants.clear();

        loadedRules.clear();

        loadedVariables.clear();

        loadedFavouriteResults.clear();

        loadedUserDefinedInstances.clear();
    }

    void ProjectLoader::checkSectionTag(const QString &currentLine)
    {
        if(currentLine.contains("Script_name"))
        {
            handleScriptNameFlags();

            loadScriptName(currentLine);

            return;
        }

        // Mark what type of information subsequent lines are holding.

        if(currentLine.contains("Variables:"))
        {
            handleVariableFlags();

            return;
        }

        if(currentLine.contains("Axiom"))
        {
            handleAxiomFlags();

            loadAxiom(currentLine);

            return;
        }

        if(currentLine.contains("Constants:"))
        {
            handleConstantFlags();

            return;
        }

        if(currentLine.contains("Rules:"))
        {
            handleRuleFlags();

            return;
        }

        if(currentLine.contains("Favourite_results:"))
        {
            handleFavouriteResultFlags();

            return;
        }

        if(currentLine.contains("User Instances:"))
        {
            handleUserInstancesFlags();

            return;
        }
    }

    void ProjectLoader::chooseLoadFunction(const QString &currentLine)
    {
        // Depending on the type of information that was being marked as being held on the next line(s),
        // load the appropriate type of information.

        if(readingVariables)
        {
            loadVariable(currentLine);
        }

        if(readingConstants)
        {
            loadConstant(currentLine);
        }

        if(readingRules)
        {
            loadRule(currentLine);
        }

        if(readingFavouriteResults)
        {
            loadFavouriteScriptResult(currentLine);
        }

        if(readingUserDefinedInstances)
        {
            loadUserAddedInstances(currentLine);
        }
    }

    void ProjectLoader::checkExpectedNumberLineTokens(int numberTokens, std::vector<int> expectedNumberTokens, const QString &fileLine) const
    {
        auto numberLocation = std::find(expectedNumberTokens.begin(), expectedNumberTokens.end(), numberTokens);

        if(numberLocation == expectedNumberTokens.end()) // Number of tokens is not a number that was expected.
        {
            throw std::runtime_error{"Unexpected formatting (wrong number of tokens) at line: " + fileLine.toStdString()};
        }
    }

    float ProjectLoader::convertNumber(const QString &stringToConvert) const
    {
        bool success;

        float numberEquivalent = stringToConvert.toFloat(&success);

        if(!success)
        {
            throw std::runtime_error{ stringToConvert.toStdString() + " is not a valid number!"};
        }

        return numberEquivalent;
    }

    ::L_System::DataStructures::StackOperation ProjectLoader::convertStackOperation(const QString &stringToConvert) const
    {
        if(stringToConvert == "Pop")
        {
            return ::L_System::DataStructures::StackOperation::Pop;
        }

        if(stringToConvert == "Push")
        {
            return ::L_System::DataStructures::StackOperation::Push;
        }

        throw std::runtime_error{stringToConvert.toStdString() + " is not a valid number!"};
    }

    void ProjectLoader::handleScriptNameFlags()
    {
        readScriptName = true;

        readingConstants = false;
        readingRules = false;
        readingVariables = false;
        readingFavouriteResults = false;

        modifiedSectionFlag = true;
    }

    void ProjectLoader::handleVariableFlags()
    {
        readingVariables = true;

        readingConstants = false;
        readingRules = false;
        readingFavouriteResults = false;

        modifiedSectionFlag = true;
    }

    void ProjectLoader::handleAxiomFlags()
    {
        readAxiom = true;

        readingConstants = false;
        readingRules = false;
        readingVariables = false;
        readingFavouriteResults = false;

        modifiedSectionFlag = true;
    }

    void ProjectLoader::handleConstantFlags()
    {
        readingConstants = true;

        readingRules = false;
        readingVariables = false;
        readingFavouriteResults = false;

        modifiedSectionFlag = true;
    }

    void ProjectLoader::handleRuleFlags()
    {
        readingRules = true;

        readingConstants = false;
        readingVariables = false;
        readingFavouriteResults = false;

        modifiedSectionFlag = true;
    }

    void ProjectLoader::handleFavouriteResultFlags()
    {
        readingFavouriteResults = true;

        readingConstants = false;
        readingRules = false;
        readingVariables = false;

        modifiedSectionFlag = true;
    }

    void ProjectLoader::handleUserInstancesFlags()
    {
        readingUserDefinedInstances = true;

        readingFavouriteResults = false;
        readingConstants = false;
        readingRules = false;
        readingVariables = false;

        modifiedSectionFlag = true;
    }

    void ProjectLoader::loadAxiom(const QString &fileLine)
    {
        QStringList lineTokens = fileLine.split(QRegExp{"\\s+"});

        /*
         * The axiom line has the following format, which when split using whitespace:
         *
         * Axiom -- variableName
         *
         * Token 0   Token 2
         *
         *  Note: the '--' and ',' are tokens but are not listed due to space limitations.
         */

        checkExpectedNumberLineTokens(lineTokens.size(), {3}, fileLine);

        // Find the Variable equivalent of the axiom name read. The project file must have written the script's
        // variables BEFORE writing the axiom.
        axiom = ::HelperFunctions::findEquivalentVariable(lineTokens[2], loadedVariables);
    }

    void ProjectLoader::loadConstant(const QString &fileLine)
    {
        QStringList lineTokens = fileLine.split(QRegExp{"\\s+"});

        /*
         *  A constant line has the following format, which when split using whitespace:
         *
         *  constantName -- stackOperation -- Translation -- translationX , translationY, translationZ
         *
         *   Token 0          Token 2            Token 4        Token 6      Token 8        Token 10
         *
         *                             OR, if constant is a rotation
         *
         *  constantName -- stackOperation -- Rotation -- rotationX , rotationY, rotationZ -- Angle -- angleNumber
         *
         *    Token 2         Token 2         Token 4     Token 6      Token 8     Token 10   Token 12    Token 14
         *
         *    Note: the '--' and ',' are tokens but are not listed due to space limitations.
         *
         *    For a translation, the required tokens to create a constant are tokens 0, 2, 4, 6, 8 and 10.
         *    For a rotation, the required tokens to create a constant are tokens 0, 2, 4, 6, 8, 10, 12, 14
         */

        checkExpectedNumberLineTokens(lineTokens.size(), {11, 15}, fileLine);

        try
        {
            ::L_System::DataStructures::StackOperation stackOperation = convertStackOperation(lineTokens[2]);

            float axisX = convertNumber(lineTokens[6]);

            float axisY = convertNumber(lineTokens[8]);

            float axisZ = convertNumber(lineTokens[10]);

            if(lineTokens[4].contains("Rotation"))
            {
                float angle = convertNumber(lineTokens[14]);

                ::L_System::DataStructures::Rotation rotation{angle, glm::vec3{axisX, axisY, axisZ}};

                loadedConstants.emplace_back(lineTokens[0], stackOperation, rotation);
            }
            else if(lineTokens[4].contains("Translation"))
            {
                ::L_System::DataStructures::Translation translation{glm::vec3{axisX, axisY, axisZ}};

                loadedConstants.emplace_back(lineTokens[0], stackOperation, translation);
            }
            else
            {
                throw std::runtime_error{"Unknown constant type encountered: " + lineTokens[4].toStdString()};
            }
        }
        catch(std::runtime_error &e)
        {
            std::string updatedErrorMessage = "The line " + fileLine.toStdString() + " could not be parsed: " + e.what();

            throw std::runtime_error{updatedErrorMessage};
        }

    }

    void ProjectLoader::loadRule(const QString &fileLine)
    {
        QStringList lineTokens = fileLine.split(QRegExp{"\\s+"});

        /*
         *  A rule line has the following format, which when split using whitespace:
         *
         *  Predecessor_name -- predecessorName
         *
         *  Token 0              Token 2
         *
         *                          OR, if the line is a successor token (type actually doesn't matter- it is stored in the file for convenience)
         *
         *  Successor_token -- Variable -- variableName
         *
         *  Token 0             Token 2     Token 4
         *
         *                       OR, if the line is a constant successor token
         *
         *  Successor_token -- Constant -- constantName
         *
         *  Token 0             Token 2     Token 4
         *
         *                      OR if the line signifies the end of a rule
         *
         *  END     RULE
         *
         *  Token 0 Token 2
         *
         *  Note: the '--' are tokens but are not listed due to space limitations in the above lines.
         */

        checkExpectedNumberLineTokens(lineTokens.size(), {2, 3, 5}, fileLine);

        static ::L_System::DataStructures::Variable equivalentVariable;
        static std::vector<::L_System::Execution::Token> successorTokens;

        if(lineTokens[0].contains("Predecessor_name"))
        {
            equivalentVariable = ::HelperFunctions::findEquivalentVariable(lineTokens[2], loadedVariables);
        }
        else if(lineTokens[0].contains("Successor_token"))
        {
            // Doesn't matter if the successor token is a constant or a variable; this function will find out which constant type the token refers to.
            successorTokens.push_back(::HelperFunctions::findEquivalentSuccessorTokens(lineTokens[4], loadedConstants, loadedVariables));
        }
        else if(fileLine.contains("END RULE"))
        {
            loadedRules.emplace_back(equivalentVariable, successorTokens);

            // Ensure that this rule's successor tokens are not in the next rule. The equivalentVariable is assigned a new
            // value when a new rule is encountered so no action has to be done there.
            successorTokens.clear();
        }
        else
        {
            throw std::runtime_error{"Unexpected file line: " + fileLine.toStdString()};
        }
    }

    void ProjectLoader::loadFavouriteScriptResult(const QString &fileLine)
    {
        QStringList lineTokens = fileLine.split(QRegExp{"\\s+"});

        /*
         * A favourite script result ine has the following format, which when split using whitespace:
         *
         * Result_Name -- resultName
         *
         * Token 0          Token 2
         *
         *              OR if the line specifies a recursion depth:
         *
         * Recursion depth -- recursionDepth
         *
         * Token 0              Token 2
         *
         *              OR if the line specifies a successor token (type of token is irrelevant- type is stored in file for convenience):
         *
         *  Successor_Token -- (Variable) -- tokenVariableName
         *
         *  Token 0             Token 2        Token 4
         *
         *              OR if the line specifies a Constant successor token:
         *
         *  Successor_Token -- (Constant) -- tokenConstantName
         *
         *  Token 0             Token 2        Token 4
         *
         *              OR if the line specifies the end of a favourite result:
         *
         *  END         FAVOURITE   RESULT
         *
         *  Token 0     Token 1     Token 2
         *
         *   Note: the '--' are tokens but are not listed due to space limitations in the above lines.
         */

        checkExpectedNumberLineTokens(lineTokens.size(), {3, 5}, fileLine);

        static QString resultName;
        static std::vector<std::vector<::L_System::Execution::Token>> successorTokens;

        if(lineTokens[0].contains("Result_name"))
        {
            resultName = lineTokens[2];
        }
        else if(lineTokens[0].contains("Recursion_depth"))
        {
            successorTokens.emplace_back();
        }
        else if(lineTokens[0].contains("Successor_token"))
        {
            successorTokens.back().emplace_back(::HelperFunctions::findEquivalentSuccessorTokens(lineTokens[4], loadedConstants, loadedVariables));
        }
        else if(fileLine.contains("END FAVOURITE RESULT"))
        {
            loadedFavouriteResults.push_back(FavouriteResult{resultName, successorTokens});

            // Ensure that this result's successor tokens are not in the next result. The resultName is assigned a new
            // value when a new result is encountered so no action has to be done there.
            successorTokens.clear();
        }
        else
        {
            throw std::runtime_error{"Unexpected file line: " + fileLine.toStdString()};
        }
    }

    void ProjectLoader::loadScriptName(const QString &fileLine)
    {
        QStringList lineTokens = fileLine.split(QRegExp{"\\s+"});

        /*
         * A name line has the following format, which when split using whitespace:
         *
         * Script_name -- scriptName
         *
         * Token 0          Token 2
         *
         *  Note: the '--' are tokens but are not listed due to space limitations in the above line.
         *
         *  The information needed to get the script name is token 2.
         */

        checkExpectedNumberLineTokens(lineTokens.size(), {3}, fileLine);

        loadedScriptName = lineTokens[2];
    }

    void ProjectLoader::loadUserAddedInstances(const QString &fileLine)
    {
        QStringList lineTokens = fileLine.split(QRegExp{"\\s+"});

        lineTokens.removeAll(",");

        /*
         * A user instance line has the following format, when split by white space:
         *
         * Model_Name -- modelName
         *
         * Token 0        Token 2
         *
         *          OR if the line represents the name of the favourite result associate with this instance
         *
         * Result_Name -- resultName
         *
         * Token 0          Token 2
         *
         *          OR if the line represents the beginning of a matrix
         *
         *  Begin_Matrix
         *
         *  Token 0
         *
         *          OR if the line represents a matrix column:
         *
         *  Value1  ,   Value2  ,   Value3  ,   Value4
         *
         *  Token 0     Token 2     Token 4     Token 6
         *
         *          OR if the line represents the end of a matrix
         *
         *  End_Matrix
         *
         *  Token 0
         *
         *          OR if the line represents the end of instance matrices for a model
         *
         * End_Model_Name
         *
         * Token 0
         *
         * Note: the '--' are tokens but are not listed due to space limitations in the above line.
         *
         */

        checkExpectedNumberLineTokens(lineTokens.size(), {1, 3, 4}, fileLine);

        static UserDefinedInstances userDefinedInstances;
        static QString associatedFavouriteResultName;
        static int columnCount = 0;
        static bool startReadMatrix = false;

        // Note: the "End_Result_Name" branch must be placed for the "Result_Name" branch as otherwise the "Result_Name"
        //       will be taken incorrectly when the file line contains "End_Result_Name",

        if(lineTokens[0].contains("Model_Name"))
        {
            userDefinedInstances.modelName = lineTokens[2];
        }
        else if(lineTokens[0].contains("End_Result_Name"))
        {
            auto favouriteResultLocation = std::find_if(loadedFavouriteResults.begin(), loadedFavouriteResults.end(), [&](const FavouriteResult favouriteResult)
            {
                return favouriteResult.resultName == associatedFavouriteResultName;
            });

            favouriteResultLocation->userDefinedInstances = loadedUserDefinedInstances;
        }
        else if(lineTokens[0].contains("Result_Name"))
        {
            associatedFavouriteResultName = lineTokens[2];
        }
        else if(lineTokens[0].contains("Begin_Matrix"))
        {
            userDefinedInstances.transformationMatrices.emplace_back(glm::mat4{1.0f});

            startReadMatrix = true;
        }
        else if(lineTokens[0].contains("End_Matrix"))
        {
            columnCount = 0;

            startReadMatrix = false;
        }
        else if(startReadMatrix)
        {
            userDefinedInstances.transformationMatrices.back()[columnCount][0] = convertNumber(lineTokens[0]);
            userDefinedInstances.transformationMatrices.back()[columnCount][1] = convertNumber(lineTokens[1]);
            userDefinedInstances.transformationMatrices.back()[columnCount][2] = convertNumber(lineTokens[2]);
            userDefinedInstances.transformationMatrices.back()[columnCount][3] = convertNumber(lineTokens[3]);

            columnCount += 1;
        }
        else if(lineTokens[0].contains("End_Model_Instances"))
        {
            loadedUserDefinedInstances.push_back(userDefinedInstances);

            userDefinedInstances.transformationMatrices.clear();
        }
        else
        {
            throw std::runtime_error{"Unexpected file line: " + fileLine.toStdString()};
        }
    }

    void ProjectLoader::loadVariable(const QString &fileLine)
    {
        QStringList lineTokens = fileLine.split(QRegExp{"\\s+"});

        /*
         * A variable line has the following format, which when split using whitespace:
         *
         * Variable_name -- variableName -- Associated_model_name -- associatedModelName
         *
         *  Token 0         Token 2               Token 4              Token 6
         *
         * Note: the '--' are tokens but are not listed due to space limitations in the above line.
         *
         *  The information needed to create a variable are tokens 2 and 6.
         */

        checkExpectedNumberLineTokens(lineTokens.size(), {7}, fileLine);

        loadedVariables.emplace_back(lineTokens[2], lineTokens[6]);
    }

    void ProjectLoader::resetFlags()
    {
        readAxiom = false;
        readingConstants = false;
        readingRules = false;
        readingVariables = false;
        readingFavouriteResults = false;
        readScriptName = false;
        readingUserDefinedInstances = false;

        modifiedSectionFlag = false;
    }
}