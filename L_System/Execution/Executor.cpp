//
// Created by binybrion on 5/6/20.
//

#include <L_System/DataStructures/Rules/Rule.h>
#include "Executor.h"
#include "../ScriptInput.h"

namespace L_System
{
    namespace Execution
    {
        std::vector<DataStructures::Rule> Executor::executionRules;
        std::vector<std::vector<Token>> Executor::recursionResult;
        std::vector<NoMatchError> Executor::noMatchErrors;

        void Executor::execute(unsigned int recursionDepth)
        {
            clearPreviousResult();

            executionRules = ScriptInput::getRules();

            recursionResult.push_back({Token{ScriptInput::getAxiom()}});

            for(unsigned int i = 0; i < recursionDepth; ++i)
            {
                std::vector<Token> currentRecursiveResult;

                bool foundMatch = false;

                for(const auto &token : recursionResult[i])
                {
                    for(const auto &rule : executionRules)
                    {
                        bool matchingRule = rule.getPredecessor() == token.getVariable();

                        if(token.isVariable() && matchingRule)
                        {
                            currentRecursiveResult.insert(currentRecursiveResult.end(), rule.getSuccessorTokens().begin(), rule.getSuccessorTokens().end());

                            foundMatch = true;

                            break;
                        }
                        else if(token.isConstant())
                        {
                            currentRecursiveResult.push_back(token);

                            foundMatch = true;

                            break;
                        }
                    }

                    if(!foundMatch)
                    {
                        noMatchErrors.push_back(NoMatchError{token, i});
                    }
                }

                recursionResult.push_back(currentRecursiveResult);
            }
        }

        const std::vector<NoMatchError> &Executor::getNoMatchErrors()
        {
            return noMatchErrors;
        }

        const std::vector<std::vector<Token>>& Executor::getRecursionResult()
        {
            return recursionResult;
        }

        // Beginning of private functions

        void Executor::clearPreviousResult()
        {
            executionRules.clear();

            noMatchErrors.clear();

            recursionResult.clear();

        }
    }
}