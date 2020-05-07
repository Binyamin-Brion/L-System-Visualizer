//
// Created by binybrion on 5/6/20.
//

#include "SuccessorToken.h"

namespace L_System
{
    namespace DataStructures
    {
        SuccessorToken::SuccessorToken(L_System::DataStructures::Constant constant)
                        :
                            constant{std::move(constant)},
                            _isConstant{true},
                            _isVariable{false}
        {

        }

        SuccessorToken::SuccessorToken(Variable variable)
                        :
                            variable{std::move(variable)},
                            _isConstant{false},
                            _isVariable{true}
        {

        }

        bool SuccessorToken::isConstant() const
        {
            return _isConstant;
        }

        bool SuccessorToken::isVariable() const
        {
            return _isVariable;
        }
    }
}