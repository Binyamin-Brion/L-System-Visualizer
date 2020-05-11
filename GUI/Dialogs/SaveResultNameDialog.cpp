//
// Created by binybrion on 5/9/20.
//

#include "SaveResultNameDialog.h"
#include "ui_saveResultNameDialog.h"
#include <QPushButton>

namespace GUI
{
    namespace Dialogs
    {
        SaveResultNameDialog::SaveResultNameDialog(QWidget *parent)
                                :
                                    QDialog{parent},
                                    ui{new Ui::SaveResultNameDialog}
        {
            ui->setupUi(this);

            setupConnections();
        }

        int SaveResultNameDialog::exec()
        {
            int executionResult = QDialog::exec();

            if(executionResult == QDialog::Accepted)
            {
                existingNames.push_back(ui->nameLineEdit->text());
            }

            ui->nameLineEdit->setText("");

            return executionResult;
        }

        QString SaveResultNameDialog::getMostRecentName() const
        {
            return existingNames.back();
        }

        bool SaveResultNameDialog::noExistingNames() const
        {
            return existingNames.empty();
        }

        void SaveResultNameDialog::removeExistingName(const QString &name)
        {
            auto nameLocation = std::find(existingNames.begin(), existingNames.end(), name);

            existingNames.erase(nameLocation);
        }

        // Beginning of private functions

        void SaveResultNameDialog::checkUniqueName(const QString &name)
        {
            auto nameLocation = std::find(existingNames.begin(), existingNames.end(), name);

            if(nameLocation == existingNames.end()) // Unique Name
            {
                ui->nameLineEdit->setStyleSheet(styleSheet());

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            }
            else
            {
                ui->nameLineEdit->setStyleSheet("background-color: rgba(255, 0, 0, 64);");

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            }
        }

        // Beginning of private functions

        void SaveResultNameDialog::setupConnections()
        {
            connect(ui->nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkUniqueName(const QString&)));
        }
    }
}