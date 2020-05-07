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
        TestExecutor::TestExecutor(const QString testClassName)
                        :
                            TestSuite{testClassName}
        {

        }

        void TestExecutor::testFractalBinaryTree()
        {
            Variable firstVariable{"0", ""};

            Variable secondVariable{"1", ""};

            Constant firstConstant{"["};

            Constant secondConstant{"]"};

            Rule rule{secondVariable, {Token{secondVariable}, Token{secondVariable}}};

            Rule secondRule{firstVariable, {{Token{secondVariable}, Token{firstConstant}, Token{firstVariable}, Token{secondConstant}, Token{firstVariable}}}};

            ::L_System::Execution::Executor executor;

            executor.setRules(std::vector<Rule>{rule, secondRule});
            executor.setAxiom(Variable{"0", ""});

            executor.execute(3);

            const std::vector<QString> expectedResult
                    {
                        "0",
                        "1[0]0",
                        "11[1[0]0]1[0]0",
                        "1111[11[1[0]0]1[0]0]11[1[0]0]1[0]0"
                    };

            auto extractedActualResult = getRecursionResults(executor);

            QString expectedSizeErrorMessage = "Expected a recursion depth level of: " + QString::number(expectedResult.size()) + " , but got: " + QString::number(extractedActualResult.size());

            QVERIFY2(extractedActualResult.size() == expectedResult.size(), qPrintable(expectedSizeErrorMessage));

            for(unsigned int i = 0; i < expectedResult.size(); ++i)
            {
                QString errorMessage = "At recursion depth: " + QString::number(i) + " expected: " + expectedResult[i] + " , but got: " + extractedActualResult[i];

                QVERIFY2(expectedResult[i] == extractedActualResult[i], qPrintable(errorMessage));
            }
        }

        std::vector<QString> TestExecutor::getRecursionResults(const ::L_System::Execution::Executor &executor)
        {
            std::vector<QString> recursionResults;

            for(const auto &recursionDepthResult : executor.getRecursionResult())
            {
                QString result;

                for(const auto &resultToken : recursionDepthResult)
                {
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
    }
}