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

        void ConstantTabContent::setupConnections()
        {
            connect(ui->addConstantButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(addConstantEntry()));

            connect(ui->deleteConstantButton, SIGNAL(clicked()), ui->scrollAreaWidgetContents, SLOT(handleDeleteButtonPushed()));
        }
    }
}