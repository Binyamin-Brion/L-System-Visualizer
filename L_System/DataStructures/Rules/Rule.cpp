//
// Created by binybrion on 5/6/20.
//

#include "Rule.h"

namespace L_System
{
    namespace DataStructures
    {
        Rule::Rule(Variable variable, std::vector<Execution::Token> successorTokens)
                :
                    predecessor{std::move(variable)},
                    successorTokens{std::move(successorTokens)}
        {

        }

        const Variable &Rule::getPredecessor() const
        {
            return predecessor;
        }

        const std::vector<Execution::Token> &Rule::getSuccessorTokens() const
        {
            return successorTokens;
        }
    }
}