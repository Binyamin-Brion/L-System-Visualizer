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

            setupConnections();
        }

        void RuleEntry::updateAvailableRuleEntries(const std::vector<QString> &variableNames, const std::vector<QString> &constantNames)
        {
            // Only variables can be the start of a rule, and so only the names stated in the variable names can are added
            // to the startComboBox.
            for(const auto &i : variableNames)
            {
                ui->startComboBox->addItem(i);
                ui->ruleComboBox->addItem(i);
            }

            for(const auto &i : constantNames)
            {
                ui->ruleComboBox->addItem(i);
            }
        }

        void RuleEntry::handleEntryVisibility(int state)
        {
            if(state == Qt::Checked)
            {
                ui->startLabel->hide();
                ui->startComboBox->hide();

                ui->ruleLabel->hide();
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

                ui->ruleLabel->show();
                ui->ruleComboBox->show();

                ui->endRuleLabel->show();
                ui->ruleProductionLabel->show();

                ui->probabilityLabel->show();
                ui->probabliltyValue->show();

                ui->undoButton->show();
            }
        }

        void RuleEntry::handleProductionChosen(const QString &text)
        {
            // After choosing a production, the initial item in the ruleComboBox is made active. To prevent the text of that
            // default item from being added to the rule's production, a check is made to ensure that never happens.
            if(text == defaultRule)
            {
                return;
            }

            // A space is added to the end of the added production to make it easier to read.
            QString addedText = text + ' ';

            ui->endRuleLabel->setText(ui->endRuleLabel->text() + addedText);

            addedProductions.push(addedText.size());

            // Set the item shown in the ruleComboBox back to the default item; this allows the user to select the same
            // production as the previous decision and still have this slot called because the index of the combo box changed.
            ui->ruleComboBox->setCurrentIndex(0);
        }

        void RuleEntry::handleUndoButton()
        {
            if(!addedProductions.empty())
            {
                QString currentProduction = ui->endRuleLabel->text();

                // This include the character included in the production for readability purposes.
                int charactersToRemove = addedProductions.top();

                currentProduction.chop(charactersToRemove);

                ui->endRuleLabel->setText(currentProduction);

                addedProductions.pop();
            }
        }

        void RuleEntry::setupConnections()
        {
            connect(ui->selectedCheckBox, &QCheckBox::stateChanged, [this](int state) { emit ruleSelected(this, state); });

            connect(ui->visibleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleEntryVisibility(int)));

            connect(ui->ruleComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(handleProductionChosen(const QString&)));

            connect(ui->undoButton, SIGNAL(clicked()), this, SLOT(handleUndoButton()));
        }
    }
}