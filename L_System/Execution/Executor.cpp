//
// Created by binybrion on 5/6/20.
//

#include <L_System/DataStructures/Rules/Rule.h>
#include "Executor.h"

namespace L_System
{
    namespace Execution
    {
        void Executor::execute(unsigned int recursionDepth)
        {
            executeRecursively(recursionDepth);
        }

        const std::vector<std::vector<Token>>& Executor::getRecursionResult() const
        {
            return recursionResult;
        }

        void Executor::setAxiom(const DataStructures::Variable &axiom)
        {
            recursionResult.push_back({Token{axiom}});
        }

        void Executor::setRules(const std::vector<DataStructures::Rule> &rules)
        {
            this->rules = rules;
        }

        void Executor::executeRecursively(unsigned int timesToExecute)
        {
            if(numberTimesExecuted == timesToExecute)
            {
                return;
            }

            std::vector<Token> currentRecursiveResult;

            for(const auto &token : recursionResult[numberTimesExecuted])
            {
                for(const auto &rule : rules)
                {
                    auto tokenName = token.getVariable().getVariableName().toStdString();
                    auto predecessorName = rule.getPredecessor().getVariableName().toStdString();

                    bool matchingRule = rule.getPredecessor() == token.getVariable();

                    if(token.isVariable() && matchingRule)
                    {
                        currentRecursiveResult.insert(currentRecursiveResult.end(), rule.getSuccessorTokens().begin(), rule.getSuccessorTokens().end());

                        goto nextLoop;
                    }
                    else if(token.isConstant())
                    {
                        currentRecursiveResult.push_back(token);

                        goto nextLoop;
                    }
                }

                assert(false);

                nextLoop:;
            }

            recursionResult.push_back(currentRecursiveResult);

            numberTimesExecuted += 1;

            executeRecursively(timesToExecute);
        }
    }
}