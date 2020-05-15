//
// Created by binybrion on 5/7/20.
//

#include <QtTest/QtTest>
#include <ext.hpp>
#include "TestInterpreter.h"
#include "../../L_System/DataStructures/Rules/Rule.h"
#include "../../L_System/Execution/Executor.h"
#include "../../L_System/Interpretation/Interpreter.h"
#include "../../L_System/ScriptInput.h"

static Tests::L_System::TestInterpreter testInterpreter{"Test_L-System-Interpreter"};

using namespace L_System;
using namespace L_System::DataStructures;
using namespace L_System::Execution;
using namespace L_System::Interpretation;

// All rules used have a probability of 100, meaning they are guaranteed to run. If they do not have a probability of 100,
// no test can be written as the output of the test cannot be known.

namespace Tests
{
    namespace L_System
    {
        TestInterpreter::TestInterpreter(const QString &testClassName)
                            : TestSuite{testClassName}
        {

        }

        void TestInterpreter::testFractalTree()
        {
            // Create variables.
            Variable firstVariable{"0", "0"};

            Variable secondVariable{"1", "1"};

            // Create constants.
            Constant firstConstant{"[", StackOperation::Push, Translation{glm::vec3{5.0f, 0.f, 0.f}}};

            Constant secondConstant{"]", StackOperation::Pop, Translation{glm::vec3{-5.0f, 0.f, 0.f}}};

            // Create rules.
            Rule rule{secondVariable, {Token{secondVariable}, Token{secondVariable}}, 100};

            Rule secondRule{firstVariable, {{Token{secondVariable}, Token{firstConstant}, Token{firstVariable}, Token{secondConstant}, Token{firstVariable}}}, 100};


            ScriptInput::setAxiom(firstVariable);
            ScriptInput::setRules({rule, secondRule});

            // Due to readability concerns, only the next two depth is tested.
            Executor::execute(2);

            Interpreter::interpret();

            testResult(createExpectedFractalResult(), __PRETTY_FUNCTION__);
        }

        void TestInterpreter::testKochCurve()
        {
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

            ScriptInput::setAxiom(variable);
            ScriptInput::setRules({rule});

            // Due to readability concerns, only the next depth is tested.
            Executor::execute(1);

            Interpreter::interpret();

            testResult(createExpectedKochCurveResult(), __PRETTY_FUNCTION__);
        }

        std::vector<std::vector<ModelInstancePlacement>> TestInterpreter::createExpectedFractalResult()
        {
            // Note: constant '[' means calling transformFractalTreeConstant().

            std::vector<std::vector<ModelInstancePlacement>> expectedTransformations;

            expectedTransformations.emplace_back();

            // First depth - 0
            glm::mat4x4 matrix = glm::mat4x4{1.0f};
            expectedTransformations.back().push_back(ModelInstancePlacement{"0", "0", matrix});

            // Second depth - 1[0]0
            expectedTransformations.emplace_back();

            matrix = glm::mat4x4{1.0f};
            expectedTransformations.back().push_back(ModelInstancePlacement{"1", "1", matrix});

            matrix = glm::mat4x4{1.f};
            transformFractalTreeConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"0", "0", matrix});

            matrix = glm::mat4{1.0f};
            expectedTransformations.back().push_back(ModelInstancePlacement{"0", "0", matrix});

            // Third depth - 11[1[0]0]1[0]0
            expectedTransformations.emplace_back();

            matrix = glm::mat4x4{1.0};
            expectedTransformations.back().push_back(ModelInstancePlacement{"1", "1", matrix});
            expectedTransformations.back().push_back(ModelInstancePlacement{"1", "1", matrix});

            transformFractalTreeConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"1", "1", matrix});

            transformFractalTreeConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"0", "0", matrix});

            matrix = glm::mat4x4{1.0};
            transformFractalTreeConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"0", "0", matrix});

            matrix = glm::mat4x4 {1.0f};
            expectedTransformations.back().push_back(ModelInstancePlacement{"1", "1", matrix});

            transformFractalTreeConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"0", "0", matrix});

            matrix = glm::mat4x4 {1.0f};
            // Since all of the ']' were matched by a '[', no transformations have to be done.
            expectedTransformations.back().push_back(ModelInstancePlacement{"0", "0", matrix});

            return expectedTransformations;
        }

        std::vector<std::vector<::L_System::Interpretation::ModelInstancePlacement>> TestInterpreter::createExpectedKochCurveResult()
        {
            // Note: constant '+' means calling transformKochCurveConstant().
            // Note: constant '-' means calling transformKochCurveSecondConstant().

            // Two functions are needed as '-' will cause an underflow, meaning that to achieve its affect
            // its transformation must be done explicitly, rather than just cancelling out a call to the transformation
            // associated with '+'.

            std::vector<std::vector<ModelInstancePlacement>> expectedTransformations;

            // First depth - F
            expectedTransformations.emplace_back();

            glm::mat4x4 matrix = glm::mat4x4{1.0f};
            expectedTransformations.back().push_back(ModelInstancePlacement{"F", "F", matrix});

            // Second depth - F+F-F-F+F
            expectedTransformations.emplace_back();
            expectedTransformations.back().push_back(ModelInstancePlacement{"F", "F", matrix});

            transformKochCurveConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"F", "F", matrix});

            transformKochCurveSecondConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"F", "F", matrix});

            // Note: due to how the interpreter works, the matrix needs to be reset to avoid compounding
            // imprecision in floating values. The effective sequence of transformations given to the model is the same however.

            matrix = glm::mat4x4{1.0f};
            transformKochCurveSecondConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"F", "F", matrix});

            // For example, stack is empty here in the interpreter. Thus this transformation will start from a "clean"
            // identity matrix, instead of a slightly off identity matrix by going from '+' -> '-' -> '-'
            //                                                                                          ^^
            //                                                                              this is a popping constant, so the
            //                                                                              stack is still empty here.
            matrix = glm::mat4x4{1.0f};
            transformKochCurveConstant(matrix);
            expectedTransformations.back().push_back(ModelInstancePlacement{"F", "F", matrix});

            return expectedTransformations;
        }

        void TestInterpreter::testResult(const std::vector<std::vector<ModelInstancePlacement>> &expectedResult, const QString &callingFunction)
        {
            // Check the interpreter interpreted the script to the correct depth.
            QString expectedSizeErrorMessage = "Expected a recursion depth level of: " + QString::number(expectedResult.size()) + " , but got: " + QString::number(Interpreter::getResult().size()) +
                                               ". Calling function: " + callingFunction;

            QVERIFY2(Interpreter::getResult().size() == expectedResult.size(), qPrintable(expectedSizeErrorMessage));

            // Check that the interpreter correctly interpreted the script at each depth level.
            for(unsigned int i = 0; i < expectedResult.size(); ++i)
            {
                // Check for the correct number of transformation matrices.
                QString elementsSizeErrorMessage = "At recursion depth: " + QString::number(i) + " expected: " + QString::number(expectedResult[i].size()) + " number of elements" +
                        " , but got: " + QString::number(Interpreter::getResult()[i].size()) + " . Calling function: " + callingFunction;

                QVERIFY2(expectedResult[i].size() == Interpreter::getResult()[i].size(), qPrintable(elementsSizeErrorMessage));

                // Check the actual value of the transformation matrices.
                for(unsigned int j = 0; j < expectedResult[i].size(); ++j)
                {
                    QString errorMessage = "At recursion depth: " + QString::number(i) + ", element " + QString::number(j) +
                                           ", the expected transformation matrix did not match. Calling function: " + callingFunction;

                    QVERIFY2(expectedResult[i][j].modelName == Interpreter::getResult()[i][j].modelName, qPrintable(errorMessage));

                    // Print a visual output of the offending matrices for easier debugging.
                    if(!(expectedResult[i][j].transformation == Interpreter::getResult()[i][j].transformation))
                    {
                        printf("%s \n\n", expectedResult[i][j].modelName.toStdString().c_str());

                        printMatrix(expectedResult[i][j].transformation);

                        printf("\n\n");

                        printMatrix(Interpreter::getResult()[i][j].transformation);
                    }

                    QVERIFY2(expectedResult[i][j].transformation == Interpreter::getResult()[i][j].transformation, qPrintable(errorMessage));
                }
            }
        }

        // These transformXXX functions must match the transformation defined for the appropriate constants in their
        // respective associated test.

        void TestInterpreter::transformFractalTreeConstant(glm::mat4x4 &matrix)
        {
            matrix = glm::translate(matrix, glm::vec3{5.f, 0.f, 0.f});
        }

        void TestInterpreter::transformKochCurveConstant(glm::mat4x4 &matrix)
        {
            matrix = glm::rotate(matrix, glm::radians(90.0f), glm::vec3{0.f, 0.f, 1.f});
        }

        void TestInterpreter::transformKochCurveSecondConstant(glm::mat4x4 &matrix)
        {
            matrix = glm::rotate(matrix, glm::radians(-90.0f), glm::vec3{0.f, 0.f, 1.f});
        }

        void TestInterpreter::printMatrix(const glm::mat4x4 &matrix)
        {
            for(unsigned int i = 0; i < 4; ++i)
            {
                printf("%f, %f, %f, %f \n", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
            }
        }
    }
}