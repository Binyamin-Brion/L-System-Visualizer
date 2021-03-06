//
// Created by binybrion on 5/6/20.
//

#include "RuleTabContent.h"
#include "ui_ruleTabContent.h"

namespace GUI
{
    namespace L_System
    {
        RuleTabContent::RuleTabContent(QWidget *parent)
                        :
                            QWidget{parent},
                            ui{new Ui::RuleTabContent}
        {
            ui->setupUi(this);

            setupConnections();
        }

        std::vector<RuleInformation> RuleTabContent::getRuleInformations() const
        {
           return ui->scrollAreaWidgetContents->getRuleInformations();
        }

        void RuleTabContent::loadRules(const std::vector<::L_System::DataStructures::Rule> &rules)
        {
            ui->scrollAreaWidgetContents->loadRules(rules);
        }

        // Beginning of public slots

        void RuleTabContent::checkForDeletedConstantUse(const std::vector<QString> &deletedConstantNames)
        {
            ui->scrollAreaWidgetContents->checkForDeletedConstantUse(deletedConstantNames);
        }

        void RuleTabContent::checkForDeletedVariableUse(const std::vector<QString> &deletedVariableNames)
        {
            ui->scrollAreaWidgetContents->checkForDeletedVariableUse(deletedVariableNames);
        }

        void RuleTabContent::updateAvailableConstantEntries(const std::vector<QString> &constantNames)
        {
            ui->scrollAreaWidgetContents->updateAvailableConstants(constantNames);
        }

        void RuleTabContent::updateAvailableVariableEntries(const std::vector<QString> &variableNames)
        {
            ui->scrollAreaWidgetContents->updateAvailableVariables(variableNames);
        }

        // Beginning of private functions

        void RuleTabContent::setupConnections()
        {
            connect(ui->addRuleButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(addRuleEntry()));

            connect(ui->deleteRuleButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(handleDeleteButtonPushed()));

            connect(ui->scrollAreaWidgetContents, &RuleWidget::allowedProbabilityChanged, [this](int value) { ui->allowedProbability->setText("Additional Probability Allowed For the Given Predecessor: " + QString::number(value)); });
        }
    }
}