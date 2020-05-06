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

        void RuleTabContent::updateAvailableRuleEntries(const std::vector<QString> &variableNames, const std::vector<QString> &constantNames)
        {
            ui->scrollAreaWidgetContents->updateAvailableRuleEntries(variableNames, constantNames);
        }

        void RuleTabContent::setupConnections()
        {
            connect(ui->addRuleButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(addRuleEntry()));

            connect(ui->deleteRuleButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(handleDeleteButtonPushed()));

            connect(ui->refreshButton, &QPushButton::clicked, [this]() { emit refreshButtonClicked(); });
        }
    }
}