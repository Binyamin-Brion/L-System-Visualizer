//
// Created by binybrion on 5/9/20.
//

#include "ProjectDetails.h"

namespace ProjectSaverLoader
{
    void ProjectDetails::addScriptInformation(const L_System::DataStructures::Variable &saveAxiom,
                                              const std::vector<::L_System::DataStructures::Constant> &saveConstants,
                                              const std::vector<::L_System::DataStructures::Rule> &saveRules,
                                              const std::vector<::L_System::DataStructures::Variable> &saveVariables)
    {
        scripts.push_back(ScriptInformation{saveAxiom, saveConstants, saveRules, saveVariables});
    }

    const std::vector<ScriptInformation> &ProjectDetails::getScripts() const
    {
        return scripts;
    }
}