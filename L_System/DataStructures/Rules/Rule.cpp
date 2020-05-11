//
// Created by binybrion on 5/6/20.
//

#include "Rule.h"

namespace L_System
{
    namespace DataStructures
    {
        Rule::Rule(Variable predecessor, std::vector<Execution::Token> successorTokens)
                :
                    predecessor{std::move(predecessor)},
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

        bool Rule::operator==(const Rule &other) const
        {
            if(successorTokens.size() != other.successorTokens.size())
            {
                return false;
            }

            for(unsigned int i = 0; i < successorTokens.size(); ++i)
            {
                if(!(successorTokens[i] == other.successorTokens[i]))
                {
                    return false;
                }
            }

            return predecessor == other.predecessor;
        }
    }
}