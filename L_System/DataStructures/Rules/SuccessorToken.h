//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_SUCCESSORTOKEN_H
#define VOXEL_L_SYSTEM_SUCCESSORTOKEN_H

#include "..//Variable.h"
#include "../Constants/Constant.h"

namespace L_System
{
    namespace DataStructures
    {
        class SuccessorToken
        {
            public:
                explicit SuccessorToken(Constant constant);
                explicit SuccessorToken(Variable variable);

                bool isConstant() const;
                bool isVariable() const;

            private:

                Constant constant;
                Variable variable;

                bool _isConstant;
                bool _isVariable;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SUCCESSORTOKEN_H
