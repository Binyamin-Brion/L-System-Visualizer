//
// Created by binybrion on 5/12/20.
//

#ifndef VOXEL_L_SYSTEM_UNDERFLOWERROR_H
#define VOXEL_L_SYSTEM_UNDERFLOWERROR_H

#include "L_System/Execution/Token.h"

namespace L_System
{
    namespace Interpretation
    {
        /**
         * Represents an underflow error that occurred during the interpretation of execution of an L-Script.
         *
         * Note: An underflow error occurs when popping the token stack when there are no tokens left.
         *       Every time a pop occurs, even at the same depth level, an underflow error is generated.
         */

        struct UnderflowError
        {
            unsigned int depthLevel;
            Execution::Token token;
        };
    }
}

#endif //VOXEL_L_SYSTEM_UNDERFLOWERROR_H
