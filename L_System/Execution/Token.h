//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_TOKEN_H
#define VOXEL_L_SYSTEM_TOKEN_H

#include "L_System/DataStructures/Constants/Constant.h"
#include "L_System/DataStructures/Variable/Variable.h"

namespace L_System
{
    namespace Execution
    {
        /**
         * Represents a part of the recursive result of an L-Script. Instances of this class are the result of matching
         * a rule, and can be used to recursively match additional rules.
         */

        class Token
        {
            public:

                /**
                 * Initializes this token with a constant. At this point the token represents a constant.
                 *
                 * @param constant  associated with this token
                 */
                explicit Token(DataStructures::Constant constant);

                /**
                * Initializes this token with a variable. At this point the token represents a variable.
                *
                * @param variable associated with this token
                */
                explicit Token(DataStructures::Variable variable);

                /**
                 * Gets the constant associated with this token, if there is one.
                 *
                 * THe function isConstant() should be called BEFORE calling this function!
                 *
                 * @return the associated constant
                 */
                const DataStructures::Constant& getConstant() const;

                /**
                * Gets the variable associated with this token, if there is one.
                *
                * THe function isVariable() should be called BEFORE calling this function!
                *
                * @return the associated constant
                */
                const DataStructures::Variable& getVariable() const;

                /**
                 * Returns whether this token was initialized with a constant.
                 *
                 * @return true if this token is associated with a constant
                 */
                bool isConstant() const;

                /**
                 * Returns whether this token was initialized with a variable.
                 *
                 * @return true if this token is associated with a variable
                 */
                bool isVariable() const;

            private:

                DataStructures::Constant constant;
                DataStructures::Variable variable;

                bool _isConstant = false;
                bool _isVariable = false;
        };
    }
}

#endif //VOXEL_L_SYSTEM_TOKEN_H
