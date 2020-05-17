//
// Created by binybrion on 5/6/20.
//

#include "RuleEntry.h"
#include "ui_ruleEntry.h"

namespace GUI
{
    namespace L_System
    {
        RuleEntry::RuleEntry(QWidget *parent)
                    :
                        QWidget{parent},
                        ui{new Ui::RuleEntry}
        {
            ui->setupUi(this);

            ui->ruleComboBox->addItem(defaultRule);

            // Make sure to set these values as Qt's default are not the same!
            ui->probabliltyValue->setMaximum(100);

            ui->probabliltyValue->setMinimum(0);

            // Set this value as when calling overloaded constructor passing in a Rule data structure this variable
            // is assigned a different value.
            previousProbabilityValue = 0;

            setupConnections();
        }

        RuleEntry::RuleEntry(const ::L_System::DataStructures::Rule &rule, QWidget *parent)
                    :
                        QWidget{parent},
                        ui{new Ui::RuleEntry}
        {
            ui->setupUi(this);

            ui->ruleComboBox->addItem(defaultRule);

            // Make sure to set these values as Qt's default are not the same!
            ui->probabliltyValue->setMaximum(100);

            ui->probabliltyValue->setMinimum(0);

            // Update internal variables to match the rule's starting probability.
            ui->probabliltyValue->setValue(rule.getProbability());

            previousProbabilityValue = rule.getProbability();

            // Call this after assigning probability value to the combo box!
            setupConnections();

            // FOr each successor token, handle it as if the user clicked the respective entry in the ruleComboBox.
            for(const auto &i : rule.getSuccessorTokens())
            {
                if(i.isConstant())
                {
                    handleProductionChosen(i.getConstant().getConstantName());
                }
                else if(i.isVariable())
                {
                    handleProductionChosen(i.getVariable().getVariableName());
                }
                else
                {
                    assert(false);
                }
            }
        }

        bool RuleEntry::checkForDeletedConstants(const std::vector<QString> &deletedConstantNames) const
        {
            for(const auto &i : addedProductionsText)
            {
                if(std::find(deletedConstantNames.begin(), deletedConstantNames.end(), i) != deletedConstantNames.end())
                {
                    return true;
                }
            }

            return false;
        }

        bool RuleEntry::checkForDeletedVariableUse(const std::vector<QString> &deletedVariableNames) const
        {
            // Since the predecessor token has to be a variable, a check to see if the predecessor token is one of the deleted
            // variables has to be done here.
            if(std::find(deletedVariableNames.begin(), deletedVariableNames.end(), ui->startComboBox->currentText()) != deletedVariableNames.end())
            {
                return true;
            }

            for(const auto &i : addedProductionsText)
            {
                if(std::find(deletedVariableNames.begin(), deletedVariableNames.end(), i) != deletedVariableNames.end())
                {
                    return true;
                }
            }

            return false;
        }

        RuleInformation RuleEntry::getRuleInformation() const
        {
          return RuleInformation{ ui->startComboBox->currentText(), addedProductionsText, static_cast<unsigned int>(ui->probabliltyValue->value())};
        }

        void RuleEntry::updateAvailableRuleEntries(std::vector<QString> constantNames, std::vector<QString> variableNames)
        {
            // While the combo boxes are updated, their indexes can change, incorrectly triggering a production to be added
            // to the rule. Thus until the combo box is done being updated, all changes to the combo box are ignored.
            ignoreChosenProductions = true;

            // Keep track of the current selection so the user does not need to respecify the starting variable name.
            QString predecessorName = ui->startComboBox->currentText();

            ui->ruleComboBox->clear();
            ui->startComboBox->clear();

            ui->ruleComboBox->addItem(defaultRule);

            for(const auto &i : constantNames)
            {
                ui->ruleComboBox->addItem(i);
            }

            // Only variables can be the start of a rule, and so only the names stated in the variable names can are added
            // to the startComboBox.
            for(const auto &i : variableNames)
            {
                ui->startComboBox->addItem(i);
                ui->ruleComboBox->addItem(i);
            }

            // Check if the starting variable is still in the updated list of variables; if it is, then it is chosen
            // to be the current selection.
            auto predecessorNameLocation = std::find(variableNames.begin(), variableNames.end(), predecessorName);

            if(predecessorNameLocation != variableNames.end())
            {
                ui->startComboBox->setCurrentIndex(std::distance(variableNames.begin(), predecessorNameLocation));
            }

            ignoreChosenProductions = false;
        }

        // Beginning of private slots

        void RuleEntry::handleEntryVisibility(int state)
        {
            if(state == Qt::Checked)
            {
                ui->startLabel->hide();
                ui->startComboBox->hide();

                ui->endRuleLabel->hide();
                ui->ruleComboBox->hide();

                ui->endRuleLabel->hide();
                ui->ruleProductionLabel->hide();

                ui->probabilityLabel->hide();
                ui->probabliltyValue->hide();

                ui->undoButton->hide();
            }
            else
            {
                ui->startLabel->show();
                ui->startComboBox->show();

                ui->endRuleLabel->show();
                ui->ruleComboBox->show();

                ui->endRuleLabel->show();
                ui->ruleProductionLabel->show();

                ui->probabilityLabel->show();
                ui->probabliltyValue->show();

                ui->undoButton->show();
            }
        }

        void RuleEntry::handleNewProbabilityValue(int value)
        {
            // To simplify implementation when changing predecessor, only allow the predecessor to be changed when the
            // probability value is 0.
            ui->startComboBox->setEnabled(value == 0);

            // Emit the signal, then update the previous probability value. This ensures that the previousProbabilityValue
            // and the current one sent in the signal are not equal to each other.
            emit probabilityValueChanged(ui->startComboBox->currentText(), this->previousProbabilityValue, value);

            previousProbabilityValue = value;
        }

        void RuleEntry::handleProductionChosen(const QString &text)
        {
            if(ignoreChosenProductions)
            {
                return;
            }

            // After choosing a production, the initial item in the ruleComboBox is made active. To prevent the text of that
            // default item from being added to the rule's production, a check is made to ensure that never happens.
            if(text == defaultRule)
            {
                return;
            }

            // A space is added to the end of the added production to make it easier to read.
            QString addedText = text + ' ';

            ui->ruleProductionLabel->setText(ui->ruleProductionLabel->text() + addedText);

            addedProductions.push(addedText.size());

            addedProductionsText.push_back(text);

            // Set the item shown in the ruleComboBox back to the default item; this allows the user to select the same
            // production as the previous decision and still have this slot called because the index of the combo box changed.
            ui->ruleComboBox->setCurrentIndex(0);
        }

        void RuleEntry::handleUndoButton()
        {
            if(!addedProductions.empty())
            {
                QString currentProduction = ui->ruleProductionLabel->text();

                // This include the character included in the production for readability purposes.
                int charactersToRemove = addedProductions.top();

                currentProduction.chop(charactersToRemove);

                ui->ruleProductionLabel->setText(currentProduction);

                addedProductions.pop();

                addedProductionsText.pop_back();
            }
        }

        void RuleEntry::setMaximumProbability(int value)
        {
            // Add the current probability value so that if the passed in number is less than the current, the current
            // value is still valid.
            ui->probabliltyValue->setMaximum(value + ui->probabliltyValue->value());
        }

        void RuleEntry::setPredecessorIndex(int index)
        {
            ui->startComboBox->setCurrentIndex(index);
        }

        // Beginning of private functions

        void RuleEntry::setupConnections()
        {
            connect(ui->probabliltyValue, SIGNAL(valueChanged(int)), this, SLOT(handleNewProbabilityValue(int)));

            connect(ui->startComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), [this](const QString&) { emit predecessorChanged(this); });

            connect(ui->selectedCheckBox, &QCheckBox::stateChanged, [this](int state) { emit ruleSelected(this, state); });

            connect(ui->visibleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleEntryVisibility(int)));

            connect(ui->ruleComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(handleProductionChosen(const QString&)));

            connect(ui->undoButton, SIGNAL(clicked()), this, SLOT(handleUndoButton()));
        }
    }
}