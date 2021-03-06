//
// Created by binybrion on 5/10/20.
//

#include <QtTest/QtTest>
#include <ext/matrix_transform.hpp>
#include "TestProjectSaveLoad.h"
#include "ProjectSaverLoader/ProjectSaver.h"
#include "ProjectSaverLoader/ProjectLoader.h"
#include "ProjectSaverLoader/ProjectDetails.h"

static Tests::ProjectSaverLoader::TestProjectSaveLoad testProjectSaveLoad{"Test_Project_Save_Load"};

using namespace L_System::DataStructures;
using namespace L_System::Execution;

// All rules used have a probability of 100, meaning they are guaranteed to run. If they do not have a probability of 100,
// no test can be written as the output of the test cannot be known.

namespace Tests
{
    namespace ProjectSaverLoader
    {
        TestProjectSaveLoad::TestProjectSaveLoad(const QString &testClassName)
                                : TestSuite{testClassName}
        {

        }

        void TestProjectSaveLoad::testSaveLoadAlgae()
        {
            const QString scriptName = "Algae";

            ::ProjectSaverLoader::ProjectDetails beforeProjectDetails;

            // Create the script.

            // Create variables.
            Variable variableA{"A", "someModel"};

            Variable variableB{"B", "someModel"};

            // Create rules.
            Rule firstRule{variableA, {Token{variableA},
                                       Token{variableB}}, 100};

            Rule secondRule{variableB, {Token{variableA}}, 100};

            // Upload the script.
            beforeProjectDetails.addScriptInformation(scriptName, variableA, {}, {firstRule, secondRule}, {variableA, variableB}, {});

            // Save and load the script.
            ::ProjectSaverLoader::ProjectSaver projectSaver;

            projectSaver.saveProject("testSaveAlgae.txt", beforeProjectDetails);

            ::ProjectSaverLoader::ProjectLoader projectLoader;

            ::ProjectSaverLoader::ProjectDetails afterProjectDetails = projectLoader.loadProject("testSaveAlgae.txt");

            // Check that the loaded script matches the script that was created in memory.

            QVERIFY(afterProjectDetails.getScripts().size() == 1); // Only one script was created and saved.

            QVERIFY(afterProjectDetails.getScripts()[0].scriptName == scriptName);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == variableA);

            QVERIFY(afterProjectDetails.getScripts()[0].variables.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[0] == variableA);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[1] == variableB);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == variableA);

            QVERIFY(afterProjectDetails.getScripts()[0].rules.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[0] == firstRule);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[1] == secondRule);

            // If there are no favourite results, then there are no user-added instances.
            QVERIFY(afterProjectDetails.getScripts()[0].favouriteResults.empty());
        }

        void TestProjectSaveLoad::testFractalTree()
        {
            const QString scriptName = "FractalTree";

            ::ProjectSaverLoader::ProjectDetails beforeProjectDetails;

            // Create the script.

            // Create variables.
            Variable firstVariable{"0", ""};

            Variable secondVariable{"1", ""};

            // Create constants.
            Constant firstConstant{"[", StackOperation::Push, Translation{glm::vec3{1, 1, 1}}};

            Constant secondConstant{"]", StackOperation::Pop, Rotation{90, glm::vec3{1, 1, 1}}};

            // Create rules.
            Rule rule{secondVariable, {Token{secondVariable}, Token{secondVariable}}, 100};

            Rule secondRule{firstVariable, {{Token{secondVariable}, Token{firstConstant}, Token{firstVariable}, Token{secondConstant}, Token{firstVariable}}}, 100};

            // Create user defined instances

            ::ProjectSaverLoader::UserDefinedInstances userDefinedInstances;
            userDefinedInstances.modelName = firstVariable.getAssociatedModelName();
            userDefinedInstances.transformationMatrices.emplace_back(glm::mat4{1.0f});
            userDefinedInstances.transformationMatrices.emplace_back(glm::mat4{1.0f});

            ::ProjectSaverLoader::UserDefinedInstances secondUserDefinedInstances;
            secondUserDefinedInstances.modelName = "someModel2";
            secondUserDefinedInstances.transformationMatrices.emplace_back(glm::mat4{1.0f});

            // Create a favourite result.
            ::ProjectSaverLoader::FavouriteResult favouriteResult
                    {
                        "firstDepthFavouriteResult",
                        {
                            {
                                Token{secondVariable}
                            },

                            {
                                Token{secondVariable},
                                Token{firstConstant},
                                Token{firstVariable},
                                Token{secondConstant},
                                Token{secondVariable}
                            }
                        },

                        {userDefinedInstances, secondUserDefinedInstances}
                    };

            // Upload the script.
            beforeProjectDetails.addScriptInformation(scriptName, firstVariable, {firstConstant, secondConstant}, {rule, secondRule}, {firstVariable, secondVariable}, {favouriteResult});

            // Save and load the script.
            ::ProjectSaverLoader::ProjectSaver projectSaver;

            projectSaver.saveProject("testSaveFractalTree.txt", beforeProjectDetails);

            ::ProjectSaverLoader::ProjectLoader projectLoader;

            ::ProjectSaverLoader::ProjectDetails afterProjectDetails = projectLoader.loadProject("testSaveFractalTree.txt");

            // Check that the loaded script matches the script that was created in memory.
            QVERIFY(afterProjectDetails.getScripts().size() == 1); // Only one script was created and saved.

            QVERIFY(afterProjectDetails.getScripts()[0].scriptName == scriptName);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == firstVariable);

            QVERIFY(afterProjectDetails.getScripts()[0].variables.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[0] == firstVariable);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[1] == secondVariable);

            QVERIFY(afterProjectDetails.getScripts()[0].constants.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].constants[0] == firstConstant);
            QVERIFY(afterProjectDetails.getScripts()[0].constants[1] == secondConstant);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == firstVariable);

            QVERIFY(afterProjectDetails.getScripts()[0].rules.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[0] == rule);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[1] == secondRule);

            QVERIFY(afterProjectDetails.getScripts()[0].favouriteResults.size() == 1);

            ::ProjectSaverLoader::FavouriteResult loadedFavouriteResult = afterProjectDetails.getScripts()[0].favouriteResults[0];

            QVERIFY(loadedFavouriteResult.resultName == "firstDepthFavouriteResult");
            QVERIFY(loadedFavouriteResult.executionResult.size() == 2);

            // Test the first depth level of the first saved result
            QVERIFY(loadedFavouriteResult.executionResult[0].size() == 1);

            // First Token
            QVERIFY(loadedFavouriteResult.executionResult[0][0].getVariable() == secondVariable);

            // Test the second depth level of the second saved result
            QVERIFY(loadedFavouriteResult.executionResult[1].size() == 5);

            // First Token
            QVERIFY(loadedFavouriteResult.executionResult[1][0].getVariable() == secondVariable);

            // Second Token
            QVERIFY(loadedFavouriteResult.executionResult[1][1].getConstant() == firstConstant);

            // Third Token
            QVERIFY(loadedFavouriteResult.executionResult[1][2].getVariable() == firstVariable);

            // Fourth Token
            QVERIFY(loadedFavouriteResult.executionResult[1][3].getConstant() == secondConstant);

            // Fifth Token
            QVERIFY(loadedFavouriteResult.executionResult[1][4].getVariable() == secondVariable);

            QVERIFY(loadedFavouriteResult.userDefinedInstances.size() == 2);

            // Test to see if the user-added model instances were correctly read from the project file.
            QVERIFY(loadedFavouriteResult.userDefinedInstances[0].modelName == firstVariable.getAssociatedModelName());
            QVERIFY(loadedFavouriteResult.userDefinedInstances[0].transformationMatrices.size() == 2);
            QVERIFY(loadedFavouriteResult.userDefinedInstances[0].transformationMatrices[0] == glm::mat4x4{1.0f});
            QVERIFY(loadedFavouriteResult.userDefinedInstances[0].transformationMatrices[1] == glm::mat4{1.0f});

            QVERIFY(loadedFavouriteResult.userDefinedInstances[1].modelName == "someModel2");
            QVERIFY(loadedFavouriteResult.userDefinedInstances[1].transformationMatrices.size() == 1);
            QVERIFY(loadedFavouriteResult.userDefinedInstances[1].transformationMatrices[0] == glm::mat4x4{1.0f});
        }

        void TestProjectSaveLoad::testKochCurve()
        {
            const QString scriptName = "KochCurve";

            // Create the script.
            ::ProjectSaverLoader::ProjectDetails beforeProjectDetails;

            // Create variables.
            Variable variable{"F", "F"};

            // Create constants.
            Constant plusConstant{"+", StackOperation::Push, Rotation{90.0f, glm::vec3{0.f, 0.f, 1.f}}};

            Constant minusConstant{"-", StackOperation::Pop, Rotation{-90.0f, glm::vec3{0.f, 0.f, 1.f}}};

            // Create rules.
            Rule rule{variable, { Token{variable},
                                  Token{plusConstant},
                                  Token{variable},
                                  Token{minusConstant},
                                  Token{variable},
                                  Token{minusConstant},
                                  Token{variable},
                                  Token{plusConstant},
                                  Token{variable}}, 100};

            // Upload the script.
            beforeProjectDetails.addScriptInformation(scriptName, variable, {plusConstant, minusConstant},{rule}, {variable}, {});

            // Save and load the script.
            ::ProjectSaverLoader::ProjectSaver projectSaver;

            projectSaver.saveProject("testSaveKochCurve.txt", beforeProjectDetails);

            ::ProjectSaverLoader::ProjectLoader projectLoader;

            ::ProjectSaverLoader::ProjectDetails afterProjectDetails = projectLoader.loadProject("testSaveKochCurve.txt");

            // Check that the loaded script matches the script that was created in memory.
            QVERIFY(afterProjectDetails.getScripts().size() == 1); // Only one script was created and saved.

            QVERIFY(afterProjectDetails.getScripts()[0].scriptName == scriptName);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == variable);

            QVERIFY(afterProjectDetails.getScripts()[0].variables.size() == 1);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[0] == variable);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == variable);

            QVERIFY(afterProjectDetails.getScripts()[0].constants.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].constants[0] == plusConstant);
            QVERIFY(afterProjectDetails.getScripts()[0].constants[1] == minusConstant);

            QVERIFY(afterProjectDetails.getScripts()[0].rules.size() == 1);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[0] == rule);

            // If there are no favourite results, then there are no user-added instances.
            QVERIFY(afterProjectDetails.getScripts()[0].favouriteResults.empty());
        }
    }
}
