//
// Created by binybrion on 5/2/20.
//

#include <GUI/L_System/Constants/ConstantsWidget.h>
#include "ScriptInfoTabWidget.h"
#include "Axiom/AxiomTabContent.h"
#include "Constants/ConstantTabContent.h"
#include "Variables/VariableTabContent.h"
#include "Rules/RuleTabContent.h"

namespace GUI
{
    namespace L_System
    {
        ScriptInfoTabWidget::ScriptInfoTabWidget(QWidget *parent)
                                :
                                    QTabWidget{parent},
                                    axiomTabContent{new AxiomTabContent{this}},
                                    constantTabContent{new ConstantTabContent{this}},
                                    ruleTabContent{new RuleTabContent{this}},
                                    variableTabContent{new VariableTabContent{this}}
        {

            addTab(variableTabContent, "Variables");
            addTab(constantTabContent, "Constants");
            addTab(ruleTabContent, "Rules");
            addTab(axiomTabContent, "Axiom");

            setupConnections();
        }

        void ScriptInfoTabWidget::setupConnections()
        {
            connect(variableTabContent, &VariableTabContent::modelLoaded, [this](const ::ModelLoading::Model &model) { emit modelLoaded(model); });

            connect(axiomTabContent, &AxiomTabContent::refreshButtonClicked, [this]() { axiomTabContent->addVariable(variableTabContent->getVariableNames()); });

            connect(ruleTabContent, &RuleTabContent::refreshButtonClicked, [this]() { ruleTabContent->updateAvailableRuleEntries(variableTabContent->getVariableNames(), constantTabContent->getConstantNames()); });
        }
    }
}
