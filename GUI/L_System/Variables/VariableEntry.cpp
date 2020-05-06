//
// Created by binybrion on 5/2/20.
//

#include "VariableEntry.h"
#include "ui_variableEntry.h"

namespace GUI
{
    namespace L_System
    {

        VariableEntry::VariableEntry(QWidget *parent)
                        :
                            QWidget{parent},
                            ui{new Ui::VariableEntry}
        {
            ui->setupUi(this);

            // By default the name is not valid, nor is the model selected as no model is selected.
            ui->variableNameLineEdit->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");

            ui->modelEntriesComboBox->addItem("No model selected.");

            ui->modelEntriesComboBox->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");

            setupConnections();
        }

        void VariableEntry::addModelEntry(const QString &modelName)
        {
            static bool noModelItemDeleted = false;

            // If this is the first model name being added, then the model name combo box will contain the item
            // "No model selected". This should be removed as after this function call, there is a model that can be selected.
            if(!noModelItemDeleted)
            {
                ui->modelEntriesComboBox->removeItem(0);

                noModelItemDeleted = true;
            }

            ui->modelEntriesComboBox->addItem(modelName);

            ui->modelEntriesComboBox->setCurrentIndex(0);

            // A valid model is now in the combo box, meaning there is no error.
            ui->modelEntriesComboBox->setStyleSheet(styleSheet());
        }

        void VariableEntry::nameValid(bool valid)
        {
            if(valid)
            {
                ui->variableNameLineEdit->setStyleSheet(styleSheet());
            }
            else
            {
                ui->variableNameLineEdit->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");
            }
        }

        void VariableEntry::setupConnections()
        {
            connect(ui->variableSelectedCheckBox, &QCheckBox::stateChanged, [this](int state) { emit variableSelected(this, state); });

            connect(ui->variableNameLineEdit, &QLineEdit::textChanged, [this](const QString &newName) { emit nameChanged(this, newName); });
        }
    }
}