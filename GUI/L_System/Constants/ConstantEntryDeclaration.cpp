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

            // By default the constant information is visible, and so the selection check box should be checked.
            // This does not work when the constant is loaded from a saved project though for some reason (this meaning
            // setting the check box to be checked upon the system creation).
            ui->selectCheckBox->setChecked(true);

            ui->transformationRadioButton->setChecked(true);

            // By default, the associated entry information is visible, and the background color of the line edit
            // where the entry name is given is highlighted red as the default name is not valid.
            ui->visibleCheckBox->setChecked(true);

            ui->nameLineEdit->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");

            setUpConnections();
        }

        ConstantEntryDeclaration::ConstantEntryDeclaration(const ::L_System::DataStructures::Constant &constant, QWidget *parent)
                                    :
                                        QWidget{parent},
                                        ui{new Ui::ConstantEntryDeclaration}
        {
            ui->setupUi(this);

            ui->nameLineEdit->setText(constant.getConstantName());

            // Enable the correct radio button based off of what type of constant entry is, and enable and disable
            // the corresponding parts of the associated information widget.
            if(constant.getRotation().isEnabled())
            {
                ui->rotationRadioButton->setChecked(true);

                ui->entryInformation->enableRotation();

                ui->entryInformation->disableTranslation();
            }
            else if(constant.getTranslation().isEnabled())
            {
                ui->transformationRadioButton->setChecked(true);

                ui->entryInformation->enableTranslation();

                ui->entryInformation->disableRotation();
            }

            // The constant holds the information for the fields of the constant declaration. Due to the fact that the
            // UI form of this object constructs the associated instance of the information widget, a separate call
            // to pass the constant data structure to the information widget is required (instead of overloading the widget's constructor).
            ui->entryInformation->setInformation(constant);

            setUpConnections();
        }

        ::L_System::DataStructures::Constant ConstantEntryDeclaration::getConstantToken() const
        {
            // A constant is either a Rotation or Translation, and depending on which one the constant is specified to be,
            // an appropriate type of constant is returned.
            if(ui->rotationRadioButton->isChecked())
            {
                return ::L_System::DataStructures::Constant
                            {
                                ui->nameLineEdit->text(),
                                ui->entryInformation->getStackOperation(),
                                ui->entryInformation->getRotation()
                            };
            }
            else
            {
                return ::L_System::DataStructures::Constant
                        {
                            ui->nameLineEdit->text(),
                            ui->entryInformation->getStackOperation(),
                            ui->entryInformation->getTranslation()
                        };
            }
        }

        bool ConstantEntryDeclaration::informationValid() const
        {
            return ui->entryInformation->informationValid();
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

        // Beginning of private slots

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

        // Beginning of private functions

        void ConstantEntryDeclaration::setUpConnections()
        {
            connect(ui->entryInformation, &ConstantEntryInformation::informationChanged, [this]() { emit informationChanged(); });

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