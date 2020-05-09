//
// Created by binybrion on 5/8/20.
//

#include "ScriptInput.h"

namespace L_System
{
    ::L_System::DataStructures::Variable ScriptInput::axiom;
    std::vector<::L_System::DataStructures::Rule> ScriptInput::rules;

    ::L_System::DataStructures::Variable ScriptInput::getAxiom()
    {
        return axiom;
    }

    const std::vector<::L_System::DataStructures::Rule> &ScriptInput::getRules()
    {
        return rules;
    }

    void ScriptInput::setAxiom(::L_System::DataStructures::Variable inputAxiom)
    {
        axiom = inputAxiom;
    }

    void ScriptInput::setRules(std::vector<::L_System::DataStructures::Rule> inputRules)
    {
        rules = inputRules;
    }
}
