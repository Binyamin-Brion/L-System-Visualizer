//
// Created by binybrion on 5/7/20.
//

#ifndef VOXEL_L_SYSTEM_INTERPRETER_H
#define VOXEL_L_SYSTEM_INTERPRETER_H

#include "ModelInstancePlacement.h"
#include "../Execution/Token.h"
#include <vector>
#include "UnbalancedError.h"
#include "UnderflowError.h"

namespace L_System
{
    namespace Interpretation
    {
        /**
         * Interprets the result of an L-Script, generating transformation matrices for variables
         * based off of the preceding constants and their (the constants') specified transformations.
         */

        class Interpreter
        {
            public:

                /**
                 * Get the result of the interpreting the result of a L-Script.
                 *
                 * @return transformation matrices to render the result of an L-Script
                 */
                static const std::vector<std::vector<ModelInstancePlacement>>& getResult();

                /**
                 * Get the list of unbalanced errors that occurred during the interpretation of the L-Script results.
                 *
                 * Note: an unbalanced error occurs when after reaching the end of the current depth result, there remains
                 *      tokens in the token stack.
                 *
                 * @return list of unbalanced errors. Each number represents a depth level where an error occurred.
                 */
                static const std::vector<UnbalancedError>& getUnBalancedErrors() ;

                /**
                 * Get the list of underflow errors that occurred during the interpretation of the L-Script results.
                 *
                 * Note: an underflow error occurs when a constant has a pop stack operation, but there are no tokens
                 *      in the token stack.
                 *
                 * @return list of underflow errors. Each number represents a depth level where an error occurred.
                 */
                static const std::vector<UnderflowError>& getUnderFlowErrors();

                /**
                 * Interpret results of executing a L-Script, using the result of executing an L-Script that is stored in
                 * the Executor.
                 */
                static void interpret();

                /**
                 * Interpret results of a previous execution of a L-Script, typically from requested to re-render a
                 * bookmarked (favourite) execution.
                 *
                 * @param previousExecutionResult the tokens generated from a previous execution of a L-Script
                 */
                static void interpretExistingResult(const std::vector<std::vector<Execution::Token>> &previousExecutionResult);

            private:

                /**
                 * Determines the transformation matrix for an instance of a variable based off of the preceding
                 * constants in the same (execution depth).
                 */
                static void calculateTransformation();

                /**
                 * Clears the previous result, if any, ensuring that the next interpretation result contains only the
                 * result of the current interpretation.
                 */
                static void clearPreviousResult();

                /**
                 * Determines how to keep track of a token based off of the stack operation of a token.
                 *
                 * @param token to process
                 * @param depthLevel the depth level that the passed in token is a part of
                 */
                static void handleToken(const Execution::Token &token, unsigned int depthLevel);

                /**
                 * Interpret results of executing a L-Script. The previous interpretation, if there is one,
                 * of a L-Script result is discarded.
                 *
                 * This function does the actual work of interpreting an execution and is called by interpret() and interpretExistingResult().
                 *
                 * @param resultTokens list of tokens generated by executing a L-Script
                 */
                static void runInterpretation(const std::vector<std::vector<Execution::Token>> &executionResult);

                static std::vector<std::vector<ModelInstancePlacement>> modelInstances;
                static std::vector<Execution::Token> tokenStack;

                static glm::mat4 transformationMatrix;

                static std::vector<UnbalancedError> unbalancedErrors;
                static std::vector<UnderflowError> underflowErrors;
        };
    }
}

#endif //VOXEL_L_SYSTEM_INTERPRETER_H
