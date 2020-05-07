//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_TESTEXECUTOR_H
#define VOXEL_L_SYSTEM_TESTEXECUTOR_H

#include "../TestSuite.h"

namespace L_System
{
    namespace Execution
    {
        class Executor;
    }
}

namespace Tests
{
    namespace L_System
    {
        /**
         * Tests the L-Script Executor to make sure that it correctly expands an script
         * correctly to a specified depth level.
         *
         * These tests are from examples on the respective Wikipedia page:
         *
         * https://en.wikipedia.org/wiki/L-system
         */

        class TestExecutor : public TestSuite
        {
            Q_OBJECT

            public:

                /**
                 *  Initializes this Test Class with the given name, allowing it to be controlled
                 *  when it is run based off of program arguments.
                 *
                 * @param testClassName name of the test class
                 */
                explicit TestExecutor(const QString &testClassName);

            private slots:

                /**
                 * Tests that the executor generates the same result as what is given on the
                 * Algae examples on the above mentioned Wikipedia page.
                 */
                void testAlgae();

                /**
                 * Tests that the executor generates the same result as what is given on the
                 * Algae examples on the above mentioned  Wikipedia page.
                 */
                void testFractalBinaryTree();

                /**
                 * Tests that the executor generates the same result as what is given on the
                 * Algae examples on the above mentioned  Wikipedia page.
                 */
                void testKochCurve();

            private:

                /**
                 * Extract the result of the executor into a vector of strings.
                 *
                 * @return vector of strings representing the result of the executor
                 */
                std::vector<QString> getRecursionResults();

                /**
                 * Compares the result of the executor with the expected result.
                 *
                 * @param expectedResult vector of strings representing the expected result of the executor
                 * @param testName name of the test calling this function
                 */
                void testResult(const std::vector<QString> &expectedResult, const QString &testName);
        };
    }
}

#endif //VOXEL_L_SYSTEM_TESTEXECUTOR_H
