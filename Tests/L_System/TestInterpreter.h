//
// Created by binybrion on 5/7/20.
//

#ifndef VOXEL_L_SYSTEM_TESTINTERPRETER_H
#define VOXEL_L_SYSTEM_TESTINTERPRETER_H

#include "../TestSuite.h"

namespace L_System
{
    namespace Interpretation
    {
        class Interpreter;
        class ModelInstancePlacement;
    }
}

namespace Tests
{
    namespace L_System
    {
        /**
         * Tests the L-Script Interpreter to make sure that it correctly calculates the transformations from the result
         * of executing an L-Script.
         *
         * These tests are from examples on the respective Wikipedia page:
         *
         * https://en.wikipedia.org/wiki/L-system
         *
         * Note: limited tests due the verbosity of testing, leading to hard to read tests. However, even the written
         *       tests are enough to test the interpreter results.
         */

        class TestInterpreter : public TestSuite
        {
            Q_OBJECT

            public:

                /**
                 *  Initializes this Test Class with the given name, allowing it to be controlled
                 *  when it is run based off of program arguments.
                 *
                 * @param testClassName name of the test class
                 */
                explicit TestInterpreter(const QString &testClassName);

            private slots:

                /**
                 * Tests that the interpreter generates the same transformations as what is given on the
                 * FractalTree example on the above mentioned Wikipedia page.
                 */
                void testFractalTree();

                /**
                 * Tests that the interpreter generates the same transformations as what is given on the
                 * KochCurve example on the above mentioned Wikipedia page.
                 */
                void testKochCurve();

            private:

                /**
                 * Generates the expected transformations for the fractal tree examples.
                 *
                 * @return list of expected transformations
                 */
                std::vector<std::vector<::L_System::Interpretation::ModelInstancePlacement>> createExpectedFractalResult();

                /**
                 * Generates the expected transformations for the koch curve tree examples.
                 *
                 * @return list of expected transformations
                 */
                std::vector<std::vector<::L_System::Interpretation::ModelInstancePlacement>> createExpectedKochCurveResult();

                /**
                 * Compares the result of the interpreter with the expected result.
                 *
                 * @param expectedResult vector of transformations representing the expected result of the interpreter
                 * @param testName name of the test calling this function
                 */
                void testResult(const std::vector<std::vector<::L_System::Interpretation::ModelInstancePlacement>> &expectedResult,
                                const QString &callingFunction);

                /**
                 * Applies the required transformation in the fractal tree example to the given matrix.
                 *
                 * @param matrix to apply the fractal tree example transformation to
                 */
                void transformFractalTreeConstant(glm::mat4x4 &matrix);

                /**
                 * Applies the required first (positive rotation) transformation in the koch curve example to the given matrix.
                 *
                 * @param matrix to apply the koch cure example transformation to
                 */
                void transformKochCurveConstant(glm::mat4x4 &matrix);

                /**
                 * Applies the required first (negative rotation) transformation in the koch curve example to the given matrix.
                 *
                 * @param matrix to apply the koch cure example transformation to
                 */
                void transformKochCurveSecondConstant(glm::mat4x4 &matrix);

                /**
                 * Prints the passed in matrix to the console.
                 *
                 * @param matrix to print to the console
                 */
                void printMatrix(const glm::mat4x4 &matrix);
        };
    }
}

#endif //VOXEL_L_SYSTEM_TESTINTERPRETER_H
