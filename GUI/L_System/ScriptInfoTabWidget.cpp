//
// Created by binybrion on 5/2/20.
//

#include "ScriptInfoTabWidget.h"
#include "Constants/ConstantTabContent.h"
#include "Variables/VariableTabContent.h"
#include "Rules/RuleTabContent.h"
#include "../../L_System/ScriptInput.h"

namespace GUI
{
    namespace L_System
    {
        ScriptInfoTabWidget::ScriptInfoTabWidget(QWidget *parent)
                                :
                                    QTabWidget{parent},
                                    constantTabContent{new ConstantTabContent{this}},
                                    ruleTabContent{new RuleTabContent{this}},
                                    variableTabContent{new VariableTabContent{this}}
        {

            addTab(variableTabContent, "Variables");
            addTab(constantTabContent, "Constants");
            addTab(ruleTabContent, "Rules");

            setupConnections();
        }

        void ScriptInfoTabWidget::setScriptInput() const
        {
            // Remember: for the execution of the script, only the axiom and the rules have to be given to the executor!
            // The constants and variables are indirectly stored in the uploaded rules.

            ::L_System::ScriptInput::setAxiom(variableTabContent->getAxiom());

            // Copies of these structures are needed to generate the rules (using the required data structures)
            // for the scripts without constantly regenerating these temporarily data structures
            std::vector<::L_System::DataStructures::Variable> scriptVariables = variableTabContent->getVariablesTokens();

            std::vector<::L_System::DataStructures::Constant> scriptConstants = constantTabContent->getConstantsTokens();

            std::vector<::L_System::DataStructures::Rule> rules;

            // Generate the rules composed of the Execution data structures
            for(const auto &i : ruleTabContent->getRuleInformations())
            {
                std::vector<::L_System::Execution::Token> predecessorTokens;

                for(const auto &production : i.productionNames)
                {
                    // Check if the production is one of the variables, and if it is, add it to the rule.
                    auto variableLocation = std::find_if(scriptVariables.begin(), scriptVariables.end(), [&](const ::L_System::DataStructures::Variable &variable)
                    {
                        return variable.getVariableName() == production;
                    });

                    if(variableLocation != scriptVariables.end())
                    {
                        predecessorTokens.emplace_back(*variableLocation);

                        continue;
                    }

                    // Check if the production is one of the constants, and if it is, add it to the rule.
                    auto constantLocations = std::find_if(scriptConstants.begin(), scriptConstants.end(), [&](const ::L_System::DataStructures::Constant &constant)
                    {
                        return constant.getConstantName() == production;
                    });

                    if(constantLocations != scriptConstants.end())
                    {
                        predecessorTokens.emplace_back(*constantLocations);

                        continue;
                    }

                    // A predecessor token must be either a variable or a constant; it is an error if it is neither.
                    assert(false);
                }

                // Check which variable matches the starting variable of the rule, and use it to create the Rule along with the
                // matched predecessor tokens in the above loop.
                auto variableLocation = std::find_if(scriptVariables.begin(), scriptVariables.end(), [&](const ::L_System::DataStructures::Variable &variable)
                {
                    return variable.getVariableName() == i.startingRuleName;
                });

                if(variableLocation != scriptVariables.end())
                {
                    rules.emplace_back(*variableLocation, predecessorTokens);

                    continue;
                }

                // A starting variable must be one of the declared variables; it is an error if this is not the case.
                assert(false);
            }

            ::L_System::ScriptInput::setRules(rules);
        }

        // Beginning of private functions

        void ScriptInfoTabWidget::setupConnections()
        {
            connect(variableTabContent, &VariableTabContent::modelLoaded, [this](const ::ModelLoading::Model &model) { emit modelLoaded(model); });

            connect(constantTabContent, SIGNAL(entryNamesChanged(std::vector<QString>)), ruleTabContent, SLOT(updateAvailableConstantEntries(std::vector<QString>)));

            connect(variableTabContent, SIGNAL(entryNamesChanged(std::vector<QString>)), ruleTabContent, SLOT(updateAvailableVariableEntries(std::vector<QString>)));
        }
    }
}
