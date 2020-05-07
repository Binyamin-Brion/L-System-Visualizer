//
// Created by binybrion on 5/6/20.
//

#include <L_System/DataStructures/Rules/Rule.h>
#include <QtTest/qtestcase.h>
#include "TestExecutor.h"
#include "../../L_System/Execution/Executor.h"

static Tests::L_System::TestExecutor testExecutor{"Test_L-System_Executor"};

using namespace L_System::DataStructures;
using namespace L_System::Execution;

namespace Tests
{
    namespace L_System
    {
        TestExecutor::TestExecutor(const QString &testClassName)
                        :
                            TestSuite{testClassName}
        {

        }

        void TestExecutor::testAlgae()
        {
            // Create variables.
            Variable variableA{"A", ""};

            Variable variableB{"B", ""};

            // Create rules.
            Rule firstRule{variableA, {Token{variableA},
                                       Token{variableB}}};

            Rule secondRule{variableB, {Token{variableA}}};

            // Executor the script.
            Executor::setAxiom(variableA);
            Executor::setRules({firstRule, secondRule});

            Executor::execute(7);

            // Test result.
            const std::vector<QString> expectedResult
                    {
                        "A",
                        "AB",
                        "ABA",
                        "ABAAB",
                        "ABAABABA",
                        "ABAABABAABAAB",
                        "ABAABABAABAABABAABABA",
                        "ABAABABAABAABABAABABAABAABABAABAAB"
                    };

            testResult(expectedResult, __PRETTY_FUNCTION__ );
        }

        void TestExecutor::testFractalBinaryTree()
        {
            // Create variables.
            Variable firstVariable{"0", ""};

            Variable secondVariable{"1", ""};

            // Create constants.
            Constant firstConstant{"["};

            Constant secondConstant{"]"};

            // Create rules.
            Rule rule{secondVariable, {Token{secondVariable}, Token{secondVariable}}};

            Rule secondRule{firstVariable, {{Token{secondVariable}, Token{firstConstant}, Token{firstVariable}, Token{secondConstant}, Token{firstVariable}}}};

            // Execute script.
            Executor::setRules(std::vector<Rule>{rule, secondRule});
            Executor::setAxiom(Variable{"0", ""});

            Executor::execute(3);

            // Test resut.
            const std::vector<QString> expectedResult
                    {
                        "0",
                        "1[0]0",
                        "11[1[0]0]1[0]0",
                        "1111[11[1[0]0]1[0]0]11[1[0]0]1[0]0"
                    };

           testResult(expectedResult, __PRETTY_FUNCTION__ );
        }

        void TestExecutor::testKochCurve()
        {
            // Create variables.
            Variable variable{"F", ""};

            // Create constants.
            Constant plusConstant{"+"};

            Constant minusConstant{"-"};

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

            // Execute script.
            Executor::setAxiom(variable);
            Executor::setRules({rule});

            Executor::execute(3);

            // Test result.
             std::vector<QString> expectedResult
                    {
                        "F",
                        "F+F-F-F+F",
                        "F+F−F−F+F+F+F−F−F+F−F+F−F−F+F−F+F−F−F+F+F+F−F−F+F",
                        "F+F−F−F+F+F+F−F−F+F−F+F−F−F+F−F+F−F−F+F+F+F−F−F+F+"
                        "F+F−F−F+F+F+F−F−F+F−F+F−F−F+F−F+F−F−F+F+F+F−F−F+F−"
                        "F+F−F−F+F+F+F−F−F+F−F+F−F−F+F−F+F−F−F+F+F+F−F−F+F−"
                        "F+F−F−F+F+F+F−F−F+F−F+F−F−F+F−F+F−F−F+F+F+F−F−F+F+"
                        "F+F−F−F+F+F+F−F−F+F−F+F−F−F+F−F+F−F−F+F+F+F−F−F+F"
                    };

             // Note: the output on the Wikipedia result does not use ASCII characters for the
             // minus symbol, leading to an incorrect failure. Thus it is replaced with an ASCII
             // version as that is what the variable is initialized with.
            for(auto &i : expectedResult)
            {
                i.replace("−", "-");
            }

            testResult(expectedResult, __PRETTY_FUNCTION__ );
        }

        std::vector<QString> TestExecutor::getRecursionResults()
        {
            std::vector<QString> recursionResults;

            // Each vector represents a result at a different depth level.
            for(const auto &recursionDepthResult : Executor::getRecursionResult())
            {
                QString result;

                for(const auto &resultToken : recursionDepthResult)
                {
                    // Each token has to be queried for its type before the name to append
                    // to the result can be done.
                    if(resultToken.isVariable())
                    {
                        result += resultToken.getVariable().getVariableName();
                    }
                    else
                    {
                        result += resultToken.getConstant().getConstantName();
                    }
                }

                recursionResults.push_back(result);
            }

            return recursionResults;
        }

        void TestExecutor::testResult(const std::vector<QString> &expectedResult, const QString &testName)
        {
            auto extractedActualResult = getRecursionResults();

            // Check the executor executed the script to the correct depth.
            QString expectedSizeErrorMessage = "Expected a recursion depth level of: " + QString::number(expectedResult.size()) + " , but got: " + QString::number(extractedActualResult.size()) +
                    ". Calling function: " + testName;

            QVERIFY2(extractedActualResult.size() == expectedResult.size(), qPrintable(expectedSizeErrorMessage));

            // Check that the executor correctly executed the script at each depth level.
            for(unsigned int i = 0; i < expectedResult.size(); ++i)
            {
                QString errorMessage = "At recursion depth: " + QString::number(i) + " expected: " + expectedResult[i] + " , but got: " + extractedActualResult[i] +
                        " . Calling function: " + testName;

                QVERIFY2(expectedResult[i] == extractedActualResult[i], qPrintable(errorMessage));
            }
        }
    }
}