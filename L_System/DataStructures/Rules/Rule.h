//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULE_H
#define VOXEL_L_SYSTEM_RULE_H

#include <vector>
#include "../Variable.h"
#include "../../Execution/Token.h"

namespace L_System
{
    namespace DataStructures
    {
        class Rule
        {
            public:
                Rule(Variable variable, std::vector<Execution::Token> successorTokens);

                const Variable& getPredecessor() const;
                const std::vector<Execution::Token>& getSuccessorTokens() const;

            private:

                Variable predecessor;
                std::vector<Execution::Token> successorTokens;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RULE_H
