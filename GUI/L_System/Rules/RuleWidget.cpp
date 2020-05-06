//
// Created by binybrion on 5/6/20.
//

#include "RuleWidget.h"
#include "ui_ruleEntry.h"
#include "RuleEntry.h"

namespace GUI
{
    namespace L_System
    {
        RuleWidget::RuleWidget(QWidget *parent)
                    :
                        QWidget{parent},
                        layout{new QVBoxLayout{this}}
        {
            setLayout(layout);
        }

        void RuleWidget::updateAvailableRuleEntries(const std::vector<QString> &variableNames, const std::vector<QString> &constantNames)
        {
            // Store a copy of the variables so that newly added rule entries can automatically have the most recent list of names to use.
            this->variableNames = variableNames;
            this->constantNames = constantNames;

            for(auto &i : rules)
            {
                i->updateAvailableRuleEntries(variableNames, constantNames);
            }
        }

        void RuleWidget::addRuleEntry()
        {
            RuleEntry *ruleEntry = new RuleEntry{this};

            // Ensure that the newly added rule entry has the most updated list of names it can be use, without having to
            // click the Refresh button first.
            ruleEntry->updateAvailableRuleEntries(variableNames, constantNames);

            connect(ruleEntry, SIGNAL(ruleSelected(RuleEntry*, int)), this, SLOT(handleRuleEntrySelected(RuleEntry*, int)));

            rules.push_back(ruleEntry);

            layout->addWidget(ruleEntry);
        }

        void RuleWidget::handleDeleteButtonPushed()
        {
            // For all of the selected entries, remove them visually, then remove them from the program.
            for(auto i : selectedRules)
            {
                layout->removeWidget(i);

                // Call delete BEFORE erasing he variable from the variables vector.
                delete i;

                auto variableLocation = std::find(rules.begin(), rules.end(), i);

                rules.erase(variableLocation);
            }

            selectedRules.clear();
        }

        void RuleWidget::handleRuleEntrySelected(RuleEntry *ruleEntry, int newState)
        {
            if(newState == Qt::Checked)
            {
                selectedRules.push_back(ruleEntry);
            }
            else
            {
                auto selectedVariableLocation = std::find(rules.begin(), rules.end(), ruleEntry);

                selectedRules.erase(selectedVariableLocation);
            }
        }
    }
}
