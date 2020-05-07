//
// Created by binybrion on 5/6/20.
//

#include <L_System/DataStructures/Rules/Rule.h>
#include "Executor.h"

namespace L_System
{
    namespace Execution
    {
        Token Executor::startingVariable{DataStructures::Variable{}};
        std::vector<DataStructures::Rule> Executor::executionRules;
        std::vector<std::vector<Token>> Executor::recursionResult;

        void Executor::execute(unsigned int recursionDepth)
        {
            recursionResult.clear();

            recursionResult.push_back({startingVariable});

            for(unsigned int i = 0; i < recursionDepth; ++i)
            {
                std::vector<Token> currentRecursiveResult;

                for(const auto &token : recursionResult[i])
                {
                    for(const auto &rule : executionRules)
                    {
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
            }
        }

        const std::vector<std::vector<Token>>& Executor::getRecursionResult()
        {
            return recursionResult;
        }

        void Executor::setAxiom(const DataStructures::Variable &axiom)
        {
            startingVariable = Token{axiom};

            recursionResult.push_back({Token{axiom}});
        }

        void Executor::setRules(const std::vector<DataStructures::Rule> &rules)
        {
            executionRules = rules;
        }
    }
}