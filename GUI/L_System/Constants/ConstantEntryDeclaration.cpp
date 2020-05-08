//
// Created by binybrion on 5/4/20.
//

#include "ConstantEntryDeclaration.h"
#include "ui_constantEntryDeclaration.h"

namespace GUI
{
    namespace L_System
    {
        /*
         * Each constant entry is composed of two parts:
         *
         * 1. The declaration
         * 2. The information
         *
         * The declaration shows the name of the entry, and holds the information associated with the entry. It also
         * contains a checkbox indicating whether to show the associated information.
         *
         * The associated information contains the actual information about the entry- how much to translate, rotate,
         * and the stack operation of the constant.
         *
         * The entry is split up into two parts to make it easier to control the visibility of the associated information.
         * Instead of many show / hide calls to the information widgets, only one show / hide call has to be made to the
         * widget holding the all of the associated information.
         */

        ConstantEntryDeclaration::ConstantEntryDeclaration(QWidget *parent)
                                    :
                                        QWidget{parent},
                                        ui{new Ui::ConstantEntryDeclaration}
        {
            ui->setupUi(this);

            ui->transformationRadioButton->setChecked(true);

            // By default, the associated entry information is visible, and the background color of the line edit
            // where the entry name is given is highlighted red as the default name is not valid.
            ui->visibleCheckBox->setChecked(true);

            ui->nameLineEdit->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");

            setUpConnections();
        }

        void ConstantEntryDeclaration::nameValid(bool valid)
        {
            if(valid)
            {
                // Restore the line edit to the default colour, indicating that the name specified is valid.
                ui->nameLineEdit->setStyleSheet(styleSheet());
            }
            else
            {
                ui->nameLineEdit->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");
            }
        }

        void ConstantEntryDeclaration::handleEntryVisibility(int state)
        {
            // If the checkbox is checked, show the associated information, otherwise hide it.
            if(state == Qt::Checked)
            {
                ui->entryInformation->show();
            }
            else
            {
                ui->entryInformation->hide();
            }
        }

        void ConstantEntryDeclaration::setUpConnections()
        {
            connect(ui->visibleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleEntryVisibility(int)));

            // The check to see if the entry name is valid can only be done in the object that owns this object, as that object holds
            // the names of the other entries.
            connect(ui->nameLineEdit, &QLineEdit::textChanged, [this](const QString &newName) { emit nameChanged(this, newName); });

            connect(ui->selectCheckBox, &QCheckBox::stateChanged, [this](int state) { emit constantEntrySelected(this, state); });

            connect(ui->rotationRadioButton, &QRadioButton::toggled, [this](bool checked) { checked ? ui->entryInformation->enableRotation() : ui->entryInformation->disableRotation(); });

            connect(ui->transformationRadioButton, &QRadioButton::toggled, [this](bool checked) { checked ? ui->entryInformation->enableTranslation() : ui->entryInformation->disableTranslation(); });
        }
    }
}