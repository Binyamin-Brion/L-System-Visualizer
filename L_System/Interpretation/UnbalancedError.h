//
// Created by binybrion on 5/12/20.
//

#ifndef VOXEL_L_SYSTEM_UNBALANCEDERROR_H
#define VOXEL_L_SYSTEM_UNBALANCEDERROR_H

#include "L_System/Execution/Token.h"
#include <vector>

namespace L_System
{
    namespace Interpretation
    {
        /**
         * Represents an unbalanced error that occurred during the interpretation of execution of an L-Script.
         *
         * Note: An unbalanced error occurs when after processing all tokens at a given depth, there remains tokens on the token stack.
         *       Every token left over in the token stack represents an Unbalanced error.
         */

        struct UnbalancedError
        {
            unsigned int depthLevel;
            std::vector<Execution::Token> tokens;
        };
    }
}

#endif //VOXEL_L_SYSTEM_UNBALANCEDERROR_H
