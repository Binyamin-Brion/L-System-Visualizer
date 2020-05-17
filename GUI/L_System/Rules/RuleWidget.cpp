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

        void RuleWidget::checkForDeletedConstantUse(const std::vector<QString> &deletedConstantNames)
        {
            std::vector<RuleEntry*> toDeleteRules;

            for(auto &i : rules)
            {
                if(i->checkForDeletedConstants(deletedConstantNames))
                {
                    toDeleteRules.push_back(i);
                }
            }

            deleteInvalidRuleEntries(toDeleteRules);
        }

        void RuleWidget::checkForDeletedVariableUse(const std::vector<QString> &deletedVariableNames)
        {
            std::vector<RuleEntry*> toDeleteRules;

            for(auto &i : rules)
            {
                if(i->checkForDeletedVariableUse(deletedVariableNames))
                {
                    toDeleteRules.push_back(i);
                }
            }

            deleteInvalidRuleEntries(toDeleteRules);
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

                connect(ruleEntry, SIGNAL(predecessorChanged(RuleEntry*)), this, SLOT(handleNewPredecessor(RuleEntry*)));

                connect(ruleEntry, SIGNAL(probabilityValueChanged(const QString&, int, int)), this, SLOT(handleNewProbabilityValue(const QString&, int, int)));

                this->rules.push_back(ruleEntry);

                layout->addWidget(ruleEntry);

                // Create a map entry for the current predecessor if it doesn't exist; otherwise update the allowed probability for rule entries
                // with the same predecessor values.
                if(!allowedProbabilityValue.contains(i.getPredecessor().getVariableName()))
                {
                    allowedProbabilityValue.insert(i.getPredecessor().getVariableName(), startingAllowedProbability - i.getProbability());
                }
                else
                {
                    allowedProbabilityValue[i.getPredecessor().getVariableName()] -= i.getProbability();
                }

                // Update the maximum allowed probability to reflect what was saved in the file.
                // Call this after the modifications to the allowedProbabilityValue made above!
                ruleEntry->setMaximumProbability( allowedProbabilityValue[i.getPredecessor().getVariableName()]);
            }
        }

        void RuleWidget::updateAvailableConstants(const std::vector<QString> &constantNames)
        {
            // Keep track of the new list of valid constant names for when a rule entry is added. See addRuleEntry() for more details.
            this->constantNames = constantNames;

            for(auto &i : rules)
            {
                i->updateAvailableRuleEntries(constantNames, variableNames);
            }
        }

        void RuleWidget::updateAvailableVariables(const std::vector<QString> &variableNames)
        {
            // Keep track of the new list of valid variable names for when a rule entry is added. See addRuleEntry() for more details.
            this->variableNames = variableNames;

            for(auto &i : rules)
            {
                i->updateAvailableRuleEntries(constantNames, variableNames);
            }

            for(const auto &i : variableNames)
            {
                if(!allowedProbabilityValue.contains(i))
                {
                    allowedProbabilityValue.insert(i, startingAllowedProbability);
                }
            }
        }

        // Beginning of public slots

        void RuleWidget::addRuleEntry()
        {
            RuleEntry *ruleEntry = new RuleEntry{this};

            // Ensure that the newly added rule entry has the most updated list of names it can be use, without having to
            // click the Refresh button first.
            ruleEntry->updateAvailableRuleEntries(constantNames, variableNames);

            ruleEntry->setMaximumProbability(startingAllowedProbability);

            connect(ruleEntry, SIGNAL(ruleSelected(RuleEntry*, int)), this, SLOT(handleRuleEntrySelected(RuleEntry*, int)));

            connect(ruleEntry, SIGNAL(predecessorChanged(RuleEntry*)), this, SLOT(handleNewPredecessor(RuleEntry*)));

            connect(ruleEntry, SIGNAL(probabilityValueChanged(const QString&, int, int)), this, SLOT(handleNewProbabilityValue(const QString&, int, int)));

            handleNewProbabilityValue(ruleEntry->getRuleInformation().startingRuleName, 0, 0);

            rules.push_back(ruleEntry);

            layout->addWidget(ruleEntry);
        }

        void RuleWidget::handleDeleteButtonPushed()
        {
            // For all of the selected entries, remove them visually, then remove them from the program.
            for(auto i : selectedRules)
            {
                // A user may delete a rule that has no starting rule, and such an entry has no value in the hashmap.
                if(!i->getRuleInformation().startingRuleName.isEmpty())
                {
                    allowedProbabilityValue[i->getRuleInformation().startingRuleName] += i->getRuleInformation().probability;
                }

                layout->removeWidget(i);

                // Call delete BEFORE erasing he variable from the variables vector.
                delete i;

                auto variableLocation = std::find(rules.begin(), rules.end(), i);

                rules.erase(variableLocation);
            }

            selectedRules.clear();
        }

        // Beginning of private slots

        void RuleWidget::handleNewPredecessor(RuleEntry *ruleEntry)
        {
            // Create a new map entry if the rule has selected a predecessor that was never selected before.
            if(!allowedProbabilityValue.contains(ruleEntry->getRuleInformation().startingRuleName))
            {
                allowedProbabilityValue.insert(ruleEntry->getRuleInformation().startingRuleName, startingAllowedProbability);
            }

            // Remember that the rule entry can only change probability values if its current probability value is 0!
            ruleEntry->setMaximumProbability(allowedProbabilityValue[ruleEntry->getRuleInformation().startingRuleName]);
        }

        void RuleWidget::handleNewProbabilityValue(const QString &predecessorName, int previousProbabilityValue, int probabilityValue)
        {
            // In case when adding a new rule entry at the beginning of a program before valid variables are specified.
            if(predecessorName.isEmpty())
            {
                return;
            }

            // This function should have been called after the handleNewPredecessor, as the predecessor automatically changes
            // to a valid variable after a valid variable is specified.
            assert(allowedProbabilityValue.contains(predecessorName));

            int changeProbabilityValue = probabilityValue - previousProbabilityValue;

            allowedProbabilityValue[predecessorName] -= changeProbabilityValue;

            // For all rules of the same predecessor, update their maximum possible probability value to reflect the change
            // in a probability value of one rule entry with the same predecessor.
            for(auto &i : rules)
            {
                if(i->getRuleInformation().startingRuleName == predecessorName)
                {
                    i->setMaximumProbability(allowedProbabilityValue[predecessorName]);
                }
            }

            emit allowedProbabilityChanged(allowedProbabilityValue[predecessorName]);
        }

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

        void RuleWidget::deleteInvalidRuleEntries(const std::vector<RuleEntry*> &invalidRules)
        {
            for(const auto &i : invalidRules)
            {
                // A user may delete a rule that has no starting rule, and such an entry has no value in the hashmap.
                if(!i->getRuleInformation().startingRuleName.isEmpty())
                {
                    allowedProbabilityValue[i->getRuleInformation().startingRuleName] += i->getRuleInformation().probability;
                }

                layout->removeWidget(i);

                // Call delete BEFORE erasing he variable from the variables vector.
                delete i;

                // Delete the variables pointing to the deleted object.

                auto ruleEntryLocation = std::find(rules.begin(), rules.end(), i);

                rules.erase(ruleEntryLocation);

                // The rule being deleted may not have been selected, thus a check has to be made if the iterator refers
                // to a valid location in the selectedRules vector.
                auto selectedRuleEntryLocation = std::find(selectedRules.begin(), selectedRules.end(), i);

                if(selectedRuleEntryLocation != selectedRules.end())
                {
                    selectedRules.erase(selectedRuleEntryLocation);
                }
            }
        }

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
