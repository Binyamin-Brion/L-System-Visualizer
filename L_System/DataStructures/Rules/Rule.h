//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULE_H
#define VOXEL_L_SYSTEM_RULE_H

#include <vector>
#include "L_System/DataStructures/Variable/Variable.h"
#include "../../Execution/Token.h"

namespace L_System
{
    namespace DataStructures
    {
        /**
         * Represents a rule in a L-Script that transforms a previous recursive depth result
         * into a new recursive result.
         */

        class Rule
        {
            public:

                /**
                 * Initializes the rule with the required information to create new recursive result.
                 *
                 * @param predecessor the variable to match against
                 * @param successorTokens the sequence of tokens to replace the matched variable with
                 * @param probability the chance of this rule being executed
                 */
                Rule(Variable predecessor, std::vector<Execution::Token> successorTokens, unsigned int probability);

                /**
                 * Get the predecessor variable that this Rule was initialized with.
                 *
                 * @return the initialized with predecessor
                 */
                const Variable& getPredecessor() const;

                /**
                 * Get the probability that this rule is executed.
                 *
                 * @return rule's probability
                 */
                unsigned int getProbability() const;

                /**
                 * Get the sequence of tokens used to replace the predecessor that was used to
                 * initialize this Rule.
                 *
                 * @return the initialized with sequence of tokens used to replace the predecessor
                 */
                const std::vector<Execution::Token>& getSuccessorTokens() const;

                bool operator==(const Rule &other) const;

            private:

                Variable predecessor;
                std::vector<Execution::Token> successorTokens;
                unsigned int probability;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RULE_H
