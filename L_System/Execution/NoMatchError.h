//
// Created by binybrion on 5/8/20.
//

#ifndef VOXEL_L_SYSTEM_NOMATCHERROR_H
#define VOXEL_L_SYSTEM_NOMATCHERROR_H

#include "../Execution/Token.h"

namespace L_System
{
    namespace Execution
    {
        /**
         *  Represents an error during the execution of a script in which a token was processed, but no matching
         *  rule was found for that token.
         */

        struct NoMatchError
        {
            Token erroneousToken;
            unsigned int depthLevel; // The depth level at which the error occurred.
        };
    }
}

#endif //VOXEL_L_SYSTEM_NOMATCHERROR_H
