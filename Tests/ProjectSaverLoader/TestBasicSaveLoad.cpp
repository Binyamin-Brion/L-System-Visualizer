//
// Created by binybrion on 5/11/20.
//

#include <QtTest/qtestcase.h>
#include "TestBasicSaveLoad.h"
#include "L_System/Execution/Token.h"
#include "ProjectSaverLoader/ProjectSaver.h"
#include "ProjectSaverLoader/ProjectLoader.h"

static Tests::ProjectSaverLoader::TestBasicSaveLoad testBasicSaveLoad{"Test_Basic_Save_Load"};

using namespace L_System::DataStructures;
using namespace L_System::Execution;

namespace Tests
{
    namespace ProjectSaverLoader
    {
        TestBasicSaveLoad::TestBasicSaveLoad(const QString &testClassName)
                            :
                                TestSuite{testClassName}
        {

        }

        void TestBasicSaveLoad::testSaveFavouriteResult()
        {
            // Create the variables used in the favourite result.
            Variable firstVariable{"something", ""};
            Variable secondVariable{"aVariable", ""};
            Variable thirdVariable{"hello", ""};

            ::ProjectSaverLoader::FavouriteResult firstFavouriteResult
                    {
                        "FavouriteResult",
                        {
                            // Save two depth results.
                            {
                                    Token{firstVariable}
                            },

                            {
                                    Token{firstVariable},
                                    Token{secondVariable}
                            },
                        }
                    };

            ::ProjectSaverLoader::FavouriteResult secondFavouriteResult
                    {
                            "SecondResult",
                            {
                                    // Save three depth results.
                                    {
                                            Token{firstVariable}
                                    },

                                    {
                                            Token{firstVariable},
                                            Token{secondVariable}
                                    },

                                    {
                                            Token{firstVariable},
                                            Token{secondVariable},
                                            Token{thirdVariable},
                                            Token{secondVariable},
                                    }
                            }
                    };

            // Save and reload the script
            ::ProjectSaverLoader::ProjectDetails projectDetails;
            projectDetails.addScriptInformation(firstVariable, {}, {}, {firstVariable, secondVariable, thirdVariable}, {firstFavouriteResult, secondFavouriteResult});

            ::ProjectSaverLoader::ProjectSaver projectSaver;
            projectSaver.saveProject("testSaveBasicFavouriteResults.txt", projectDetails);

            ::ProjectSaverLoader::ProjectLoader projectLoader;
            ::ProjectSaverLoader::ProjectDetails afterProjectDetails = projectLoader.loadProject("testSaveBasicFavouriteResults.txt");

            // Verify that the saved favourite results are loaded correctly

            QVERIFY(afterProjectDetails.getScripts().size() == 1);

            QVERIFY(afterProjectDetails.getScripts()[0].favouriteResults.size() == 2);

            // First Saved Result

            ::ProjectSaverLoader::FavouriteResult favouriteResult = afterProjectDetails.getScripts()[0].favouriteResults[0];

            QVERIFY(favouriteResult.resultName == "FavouriteResult");
            QVERIFY(favouriteResult.executionResult.size() == 2);

            // Test the first depth level of the first saved result
            QVERIFY(favouriteResult.executionResult[0].size() == 1);
            QVERIFY(favouriteResult.executionResult[0][0].isVariable());
            QVERIFY(favouriteResult.executionResult[0][0].getVariable().getVariableName() == "something");

            // Test the second depth level of the first saved result
            QVERIFY(favouriteResult.executionResult[1].size() == 2);

            // First Token
            QVERIFY(favouriteResult.executionResult[1][0].isVariable());
            QVERIFY(favouriteResult.executionResult[1][0].getVariable().getVariableName() == "something");

            // Second Token
            QVERIFY(favouriteResult.executionResult[1][1].isVariable());
            QVERIFY(favouriteResult.executionResult[1][1].getVariable().getVariableName() == "aVariable");


            // Second Saved Result

            ::ProjectSaverLoader::FavouriteResult secondResult = afterProjectDetails.getScripts()[0].favouriteResults[1];

            QVERIFY(secondResult.resultName == "SecondResult");
            QVERIFY(secondResult.executionResult.size() == 3);

            // The first two depth levels in the second result are the same as the First result

            // Test the first depth level of the second saved result
            QVERIFY(secondResult.executionResult[0].size() == 1);

            // First Token
            QVERIFY(secondResult.executionResult[0][0].isVariable());
            QVERIFY(secondResult.executionResult[0][0].getVariable().getVariableName() == "something");

            // Test the second depth level of the second saved result
            QVERIFY(secondResult.executionResult[1].size() == 2);

            // First Token
            QVERIFY(secondResult.executionResult[1][0].isVariable());
            QVERIFY(secondResult.executionResult[1][0].getVariable().getVariableName() == "something");

            // Second Token
            QVERIFY(secondResult.executionResult[1][1].isVariable());
            QVERIFY(secondResult.executionResult[1][1].getVariable().getVariableName() == "aVariable");

            // Test the third depth level of the second saved result

            // First Token
            QVERIFY(secondResult.executionResult[2][0].isVariable());
            QVERIFY(secondResult.executionResult[2][0].getVariable().getVariableName() == "something");

            // Second Token
            QVERIFY(secondResult.executionResult[2][1].isVariable());
            QVERIFY(secondResult.executionResult[2][1].getVariable().getVariableName() == "aVariable");

            // Third Token
            QVERIFY(secondResult.executionResult[2][2].isVariable());
            QVERIFY(secondResult.executionResult[2][2].getVariable().getVariableName() == "hello");

            // Fourth Token
            QVERIFY(secondResult.executionResult[2][3].isVariable());
            QVERIFY(secondResult.executionResult[2][3].getVariable().getVariableName() == "aVariable");

        }
    }
}