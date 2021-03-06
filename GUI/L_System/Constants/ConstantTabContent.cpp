//
// Created by binybrion on 5/3/20.
//

#include "ConstantTabContent.h"
#include "ui_constantTabContent.h"
#include <QScrollBar>
#include <QWheelEvent>

namespace GUI
{
    namespace L_System
    {
        ConstantTabContent::ConstantTabContent(QWidget *parent)
                            :
                                QWidget{parent},
                                ui{new Ui::ConstantTabContent}
        {
            ui->setupUi(this);

            setupConnections();
        }

        std::vector<::L_System::DataStructures::Constant> ConstantTabContent::getConstantsTokens() const
        {
            return ui->scrollAreaWidgetContents->getConstantsTokens();
        }

        void ConstantTabContent::loadEntries(const std::vector<::L_System::DataStructures::Constant> &constants)
        {
            ui->scrollAreaWidgetContents->loadEntries(constants);
        }

        // Beginning of public slots

        void ConstantTabContent::setVariableNames(const std::vector<QString> &variableNames)
        {
            ui->scrollAreaWidgetContents->setVariableNames(variableNames);
        }

        // Beginning of private functions

        void ConstantTabContent::setupConnections()
        {
            connect(ui->addConstantButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(addConstantEntry()));

            connect(ui->deleteConstantButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(handleDeleteButtonPushed()));

            connect(ui->scrollAreaWidgetContents, &ConstantsWidget::constantsChangedValidity, [this](const std::vector<QString> &constantNames) { emit constantsChangedValidity(constantNames); });

            connect(ui->scrollAreaWidgetContents, &ConstantsWidget::constantsDeleted, [this](const std::vector<QString> &deletedConstantNames) { emit constantsDeleted(deletedConstantNames); });
        }
    }
}