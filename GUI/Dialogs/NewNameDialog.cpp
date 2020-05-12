//
// Created by binybrion on 5/9/20.
//

#include "NewNameDialog.h"
#include "ui_newNameDialog.h"
#include <QPushButton>

namespace GUI
{
    namespace Dialogs
    {
        NewNameDialog::NewNameDialog(QWidget *parent)
                                :
                                    QDialog{parent},
                                    ui{new Ui::NewNameDialog}
        {
            ui->setupUi(this);

            setupConnections();
        }

        void NewNameDialog::addExistingName(const QString &name)
        {
            existingNames.push_back(name);
        }

        void NewNameDialog::clearExistingName()
        {
            existingNames.clear();
        }

        int NewNameDialog::exec()
        {
            int executionResult = QDialog::exec();

            if(executionResult == QDialog::Accepted)
            {
                existingNames.push_back(ui->nameLineEdit->text());
            }

            ui->nameLineEdit->setText("");

            return executionResult;
        }

        QString NewNameDialog::getMostRecentName() const
        {
            return existingNames.back();
        }

        bool NewNameDialog::noExistingNames() const
        {
            return existingNames.empty();
        }

        void NewNameDialog::removeExistingName(const QString &name)
        {
            auto nameLocation = std::find(existingNames.begin(), existingNames.end(), name);

            existingNames.erase(nameLocation);
        }

        // Beginning of private functions

        void NewNameDialog::checkUniqueName(const QString &name)
        {
            auto nameLocation = std::find(existingNames.begin(), existingNames.end(), name);

            if(nameLocation == existingNames.end()) // Unique Name
            {
                ui->nameLineEdit->setStyleSheet(styleSheet());

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            }
            else
            {
                // Visually display an error as a red back-ground and do not allow user to check the "Ok" button.

                ui->nameLineEdit->setStyleSheet("background-color: rgba(255, 0, 0, 64);");

                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            }
        }

        // Beginning of private functions

        void NewNameDialog::setupConnections()
        {
            connect(ui->nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkUniqueName(const QString&)));
        }
    }
}