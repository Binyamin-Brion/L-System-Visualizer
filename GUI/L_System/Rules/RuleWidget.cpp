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

        std::vector<RuleInformation> RuleWidget::getRuleInformations() const
        {
            std::vector<RuleInformation> ruleInformations;

            for(const auto &i : rules)
            {
                ruleInformations.push_back(i->getRuleInformation());
            }

            return ruleInformations;
        }

        void RuleWidget::loadRules(const std::vector<::L_System::DataStructures::Rule> &rules)
        {
            removeExistingEntries();

            for(const auto &i : rules)
            {
                RuleEntry *ruleEntry = new RuleEntry{i, this};

                // Ensure that the newly added rule entry has the most updated list of names it can be use, without having to
                // click the Refresh button first.
                ruleEntry->updateAvailableRuleEntries(constantNames, variableNames);

                // Show the appropriate predecessor name in the predecessor combo box. This must be called AFTER
                // calling the updateAvailableRuleEntries().
                ruleEntry->setPredecessorIndex(findPredecessorIndex(i.getPredecessor().getVariableName()));

                connect(ruleEntry, SIGNAL(ruleSelected(RuleEntry*, int)), this, SLOT(handleRuleEntrySelected(RuleEntry*, int)));

                this->rules.push_back(ruleEntry);

                layout->addWidget(ruleEntry);
            }
        }

        void RuleWidget::updateAvailableConstants(std::vector<QString> constantNames)
        {
            // Keep track of the new list of valid constant names for when a rule entry is added. See addRuleEntry() for more details.
            this->constantNames = constantNames;

            for(auto &i : rules)
            {
                i->updateAvailableRuleEntries(constantNames, variableNames);
            }
        }

        void RuleWidget::updateAvailableVariables(std::vector<QString> variableNames)
        {
            // Keep track of the new list of valid variable names for when a rule entry is added. See addRuleEntry() for more details.
            this->variableNames = variableNames;

            for(auto &i : rules)
            {
                i->updateAvailableRuleEntries(constantNames, variableNames);
            }
        }

        // Beginning of public slots

        void RuleWidget::addRuleEntry()
        {
            RuleEntry *ruleEntry = new RuleEntry{this};

            // Ensure that the newly added rule entry has the most updated list of names it can be use, without having to
            // click the Refresh button first.
            ruleEntry->updateAvailableRuleEntries(constantNames, variableNames);

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

        // Beginning of private slots

        void RuleWidget::handleRuleEntrySelected(RuleEntry *ruleEntry, int newState)
        {
            if(newState == Qt::Checked)
            {
                selectedRules.push_back(ruleEntry);
            }
            else
            {
                auto selectedVariableLocation = std::find(selectedRules.begin(), selectedRules.end(), ruleEntry);

                selectedRules.erase(selectedVariableLocation);
            }
        }

        // Beginning of private functions

        int RuleWidget::findPredecessorIndex(const QString &variableName) const
        {
            int index = 0;

            for(const auto &i : variableNames)
            {
                if(i == variableName)
                {
                    return index;
                }

                index += 1;
            }

            assert(false);
        }

        void RuleWidget::removeExistingEntries()
        {
            QLayoutItem *layoutItem = nullptr;

            while((layoutItem = layout->takeAt(0)) != nullptr)
            {
                delete layoutItem->widget();
                delete layoutItem;
            }

            rules.clear();

            selectedRules.clear();
        }
    }
}
