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

                ui->errorButton->setEnabled(false);
            }

            void ScriptOutputEntry::appendResultText(const QString &text)
            {
                ui->outputLabel->setText(ui->outputLabel->text() + " " + text);
            }

            void ScriptOutputEntry::setDepthResult(unsigned int depthResult)
            {
                ui->recursionDepthLabel->setText("Recursion depth: " + QString::number(depthResult) + ". ");
            }

            void ScriptOutputEntry::setErrorMessage(const QString &errorMessage)
            {
                // There is no way to undo setting an error message, which makes sense as the result of script cannot be
                // changed unless the script itself is changed, in which case the script is executed again and a new set
                // of result entries are created.

                ui->errorButton->setStyleSheet("background-color: rgba(128, 0, 0, 32)");

                ui->errorButton->setText(errorMessage);
            }
        }
    }
}
