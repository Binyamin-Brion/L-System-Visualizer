//
// Created by binybrion on 5/6/20.
//

#include "Token.h"

namespace L_System
{
    namespace Execution
    {
        Token::Token(DataStructures::Constant constant)
                :
                    constant{std::move(constant)},
                    _isConstant{true}
        {

        }

        Token::Token(DataStructures::Variable variable)
                :
                    variable{std::move(variable)},
                    _isVariable{true}
        {

        }

        const DataStructures::Constant &Token::getConstant() const
        {
            return constant;
        }

        const DataStructures::Variable &Token::getVariable() const
        {
            return variable;
        }

        bool Token::isConstant() const
        {
            return _isConstant;
        }

        bool Token::isVariable() const
        {
            return _isVariable;
        }

        bool Token::operator==(const Token &other) const
        {
            if(_isConstant)
            {
                return other.isConstant() && constant == other.getConstant();
            }
            else
            {
                return other.isVariable() && variable == other.getVariable();
            }
        }
    }

}