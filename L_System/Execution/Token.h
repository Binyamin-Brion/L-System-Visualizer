//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_TOKEN_H
#define VOXEL_L_SYSTEM_TOKEN_H

#include "L_System/DataStructures/Constants/Constant.h"
#include "L_System/DataStructures/Variable.h"

namespace L_System
{
    namespace Execution
    {
        class Token
        {
            public:

                explicit Token(DataStructures::Constant constant);
                explicit Token(DataStructures::Variable variable);

                const DataStructures::Constant& getConstant() const;
                const DataStructures::Variable& getVariable() const;

                bool isConstant() const;
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
