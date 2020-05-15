//
// Created by binybrion on 5/6/20.
//

#include <L_System/DataStructures/Rules/Rule.h>
#include "Executor.h"
#include "../ScriptInput.h"
#include <QHash>
#include <random>
#include <functional>

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

            QHash<QString, std::vector<DataStructures::Rule>> rules;

            for(const auto &i : ScriptInput::getRules())
            {
                if(!rules.contains(i.getPredecessor().getVariableName()))
                {
                    rules.insert(i.getPredecessor().getVariableName(), std::vector<DataStructures::Rule>{});
                }

                rules[i.getPredecessor().getVariableName()].push_back(i);
            }

            executionRules = ScriptInput::getRules();

            recursionResult.push_back({Token{ScriptInput::getAxiom()}});

            for(unsigned int i = 0; i < recursionDepth; ++i)
            {
                std::vector<Token> currentRecursiveResult;

                bool foundMatch = false;

                for(const auto &token : recursionResult[i])
                {
                    // For every rule, choose a rule. If the chosen rule does not match the token, then the result of choosing
                    // a rule has no effect.
                    for(const auto &rule : rules)
                    {
                        ChooseRuleResult chosenRule = chooseRule(rule);

                        // This value has no meaning if the token is not a variable. Does not matter though as a check
                        // to make sure the token is a variable before executing the rule is done.
                        // Also has no meaning if no rule was chosen.
                        bool matchingRule = chosenRule.ruleChosen && chosenRule.chosenRule.getPredecessor() == token.getVariable();

                        if(token.isVariable() && matchingRule)
                        {
                            currentRecursiveResult.insert(currentRecursiveResult.end(), chosenRule.chosenRule.getSuccessorTokens().begin(), chosenRule.chosenRule.getSuccessorTokens().end());

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

        Executor::ChooseRuleResult Executor::chooseRule(const std::vector<DataStructures::Rule> &rules)
        {
            // This function works by putting instances of each rule into a vector; the number of times they are placed
            // is equal to their probability. Then a random number is chosen indicating an index into that vector, indicating
            // the rule to return.

            // A rule here always has at least a one percent chance of occurring, due to the restriction in the ScriptInfoTab.cpp.
            // Thus the first element of the sortedRules is always valid.

            // If the sum of the probabilities of the rules are greater than 100, an invariant flag is set, so that the user
            // can be notified of an error in the program after the execution done. In this case, some rules whose indexes
            // end up being greater than 99 will never get chosen.

            std::random_device randomDevice;
            std::mt19937 generator(randomDevice());

            // Remember indexes start at 0!
            std::uniform_int_distribution<> distribution(0, 99);

            std::vector<std::reference_wrapper<const DataStructures::Rule>> sortedRules;

            // Add references to each rule, equal to the rule's probability.
            for(const auto &i : rules)
            {
                for(unsigned int index = 0; index < i.getProbability(); ++index)
                {
                    sortedRules.push_back(std::cref(i));
                }
            }

            // The rule to choose.
            unsigned int generatedValue = distribution(generator);

            // If the sum of the rules does not equal to 100, then it is assumed the user wants nothing to happen in that
            // case. Thus a a value of false is returned.
            if(generatedValue >= sortedRules.size())
            {
                return {false, rules.front()};
            }
            else
            {
                return {true, sortedRules[generatedValue].get()};
            }
        }
    }
}