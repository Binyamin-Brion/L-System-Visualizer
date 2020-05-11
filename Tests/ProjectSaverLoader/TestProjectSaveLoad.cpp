//
// Created by binybrion on 5/10/20.
//

#include <QtTest/QtTest>
#include "TestProjectSaveLoad.h"
#include "ProjectSaverLoader/ProjectSaver.h"
#include "ProjectSaverLoader/ProjectLoader.h"
#include "ProjectSaverLoader/ProjectDetails.h"

static Tests::ProjectSaverLoader::TestProjectSaveLoad testProjectSaveLoad{"Test_Project_Save_Load"};

using namespace L_System::DataStructures;
using namespace L_System::Execution;

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
            ::ProjectSaverLoader::ProjectDetails beforeProjectDetails;

            // Create the script.

            // Create variables.
            Variable variableA{"A", "someModel"};

            Variable variableB{"B", "someModel"};

            // Create rules.
            Rule firstRule{variableA, {Token{variableA},
                                       Token{variableB}}};

            Rule secondRule{variableB, {Token{variableA}}};

            // Upload the script.
            beforeProjectDetails.addScriptInformation(variableA, {}, {firstRule, secondRule}, {variableA, variableB});

            // Save and load the script.
            ::ProjectSaverLoader::ProjectSaver projectSaver;

            projectSaver.saveProject("testSaveAlgae.txt", beforeProjectDetails);

            ::ProjectSaverLoader::ProjectLoader projectLoader;

            ::ProjectSaverLoader::ProjectDetails afterProjectDetails = projectLoader.loadProject("testSaveAlgae.txt");

            // Check that the loaded script matches the script that was created in memory.
            QVERIFY(afterProjectDetails.getScripts().size() == 1); // Only one script was created and saved.

            QVERIFY(afterProjectDetails.getScripts()[0].variables.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[0] == variableA);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[1] == variableB);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == variableA);

            QVERIFY(afterProjectDetails.getScripts()[0].rules.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[0] == firstRule);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[1] == secondRule);
        }

        void TestProjectSaveLoad::testFractalTree()
        {
            ::ProjectSaverLoader::ProjectDetails beforeProjectDetails;

            // Create the script.

            // Create variables.
            Variable firstVariable{"0", ""};

            Variable secondVariable{"1", ""};

            // Create constants.
            Constant firstConstant{"[", StackOperation::Push, Translation{glm::vec3{1, 1, 1}}};

            Constant secondConstant{"]", StackOperation::Pop, Rotation{90, glm::vec3{1, 1, 1}}};

            // Create rules.
            Rule rule{secondVariable, {Token{secondVariable}, Token{secondVariable}}};

            Rule secondRule{firstVariable, {{Token{secondVariable}, Token{firstConstant}, Token{firstVariable}, Token{secondConstant}, Token{firstVariable}}}};

            // Upload the script.
            beforeProjectDetails.addScriptInformation(firstVariable, {firstConstant, secondConstant}, {rule, secondRule}, {firstVariable, secondVariable});

            // Save and load the script.
            ::ProjectSaverLoader::ProjectSaver projectSaver;

            projectSaver.saveProject("testSaveFractalTree.txt", beforeProjectDetails);

            ::ProjectSaverLoader::ProjectLoader projectLoader;

            ::ProjectSaverLoader::ProjectDetails afterProjectDetails = projectLoader.loadProject("testSaveFractalTree.txt");

            // Check that the loaded script matches the script that was created in memory.
            QVERIFY(afterProjectDetails.getScripts().size() == 1); // Only one script was created and saved.

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
        }

        void TestProjectSaveLoad::testKochCurve()
        {
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
                                  Token{variable}}};

            // Upload the script.
            beforeProjectDetails.addScriptInformation(variable, {plusConstant, minusConstant},{rule}, {variable});

            // Save and load the script.
            ::ProjectSaverLoader::ProjectSaver projectSaver;

            projectSaver.saveProject("testSaveKochCurve.txt", beforeProjectDetails);

            ::ProjectSaverLoader::ProjectLoader projectLoader;

            ::ProjectSaverLoader::ProjectDetails afterProjectDetails = projectLoader.loadProject("testSaveKochCurve.txt");

            // Check that the loaded script matches the script that was created in memory.
            QVERIFY(afterProjectDetails.getScripts().size() == 1); // Only one script was created and saved.

            QVERIFY(afterProjectDetails.getScripts()[0].variables.size() == 1);
            QVERIFY(afterProjectDetails.getScripts()[0].variables[0] == variable);

            QVERIFY(afterProjectDetails.getScripts()[0].axiom == variable);

            QVERIFY(afterProjectDetails.getScripts()[0].constants.size() == 2);
            QVERIFY(afterProjectDetails.getScripts()[0].constants[0] == plusConstant);
            QVERIFY(afterProjectDetails.getScripts()[0].constants[1] == minusConstant);

            QVERIFY(afterProjectDetails.getScripts()[0].rules.size() == 1);
            QVERIFY(afterProjectDetails.getScripts()[0].rules[0] == rule);
        }
    }
}
