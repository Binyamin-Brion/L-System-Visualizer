//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_EXECUTOR_H
#define VOXEL_L_SYSTEM_EXECUTOR_H

#include <QString>
#include <vector>

#include "..//DataStructures/Constants/Constant.h"
#include "../DataStructures/Variable/Variable.h"
#include "../DataStructures/Rules/Rule.h"
#include "Token.h"
#include "NoMatchError.h"

namespace L_System
{
    namespace Execution
    {
        /**
         * Executes a L-Script, expanding the output to the specified depth level.
         */

        class Executor
        {
            public:

                /**
                 * Executes the script to the passed in depth level.
                 *
                 * Both axiom and the rules must have been set using the appropriate functions for any valid result.
                 *
                 * Note: the previous execution result is discarded (if there is one) when calling this function.
                 *
                 * @param recursionDepth depth to which to execute the script
                 */
                static void execute(unsigned int recursionDepth);

                /**
                 * Get the errors that occurred when running the script due to no rule matching the current token.
                 *
                 * @return list of all no matching rule errors that occurred during the execution of the script
                 */
                static const std::vector<NoMatchError>& getNoMatchErrors();

                /**
                 * Get the result of running the script.
                 *
                 * @return script result
                 */
                static const std::vector<std::vector<Token>>& getRecursionResult();

            private:

                struct ChooseRuleResult
                {
                    bool ruleChosen;
                    const DataStructures::Rule& chosenRule;
                };

                /**
                 * Chooses a rule from the vector rules based off of their probabilities.
                 *
                 * See note in Executor.cpp.
                 *
                 * @param rules from which to randomly choose a probability
                 * @return result of choosing a rule
                 */
                static ChooseRuleResult chooseRule(const std::vector<DataStructures::Rule>& rules);

                /**
                 * Clears the previous result, if any, ensuring that the next execution result contains only the
                 * result of the current execution.
                 */
                static void clearPreviousResult();

                static std::vector<DataStructures::Rule> executionRules;
                static std::vector<std::vector<Token>> recursionResult;

                static std::vector<NoMatchError> noMatchErrors;
        };
    }
}

#endif //VOXEL_L_SYSTEM_EXECUTOR_H
