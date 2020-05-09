//
// Created by binybrion on 5/8/20.
//

#include "ScriptOutputEntry.h"
#include "ui_scriptOutputEntry.h"

namespace GUI
{
    namespace Output
    {
        namespace L_System
        {
            ScriptOutputEntry::ScriptOutputEntry(QWidget *parent)
                    :
                    QWidget{parent},
                    ui{new Ui::ScriptOutputEntry}
            {
                ui->setupUi(this);
            }

            void ScriptOutputEntry::appendResultText(const QString &text)
            {
                ui->outputLabel->setText(ui->outputLabel->text() + " " + text);
            }

            void ScriptOutputEntry::setDepthResult(unsigned int depthResult)
            {
                ui->recursionDepthLabel->setText("Recursion depth: " + QString::number(depthResult) + ". ");
            }
        }
    }
}
