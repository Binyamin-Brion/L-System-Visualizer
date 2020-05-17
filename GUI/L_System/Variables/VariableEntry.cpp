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

            ui->modelEntriesComboBox->addItem(noModelText);

            ui->modelEntriesComboBox->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");

            setupConnections();
        }

        VariableEntry::VariableEntry(const ::L_System::DataStructures::Variable &variable, QWidget *parent)
                        :
                            QWidget{parent},
                            ui{new Ui::VariableEntry}
        {
            ui->setupUi(this);

            ui->variableNameLineEdit->setText(variable.getVariableName());

            setupConnections();
        }

        void VariableEntry::addModelEntry(const QString &modelName)
        {
            // Ensure that after adding an entry to the combo box, the old entry remains.
            int currentIndex = ui->modelEntriesComboBox->currentIndex();

            ui->modelEntriesComboBox->addItem(modelName);

            ui->modelEntriesComboBox->setCurrentIndex(currentIndex);
        }

        QString VariableEntry::getAssociatedModelName() const
        {
            return ui->modelEntriesComboBox->currentText();
        }

        const QString &VariableEntry::getNoModelText() const
        {
            return noModelText;
        }

        ::L_System::DataStructures::Variable VariableEntry::getVariableToken() const
        {
            return ::L_System::DataStructures::Variable
                    {
                        ui->variableNameLineEdit->text(),
                        ui->modelEntriesComboBox->currentText()
                    };
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

        void VariableEntry::setAssociatedModelIndex(int index)
        {
            ui->modelEntriesComboBox->setCurrentIndex(index);
        }

        // Beginning of private slots

        void VariableEntry::handleDifferentAssociatedModelSelected(const QString &modelName)
        {
            if(modelName == noModelText)
            {
                ui->modelEntriesComboBox->setStyleSheet("background-color: rgba(192, 0, 0, 0.2);");
            }
            else
            {
                // A valid model is now in the combo box, meaning there is no error.
                ui->modelEntriesComboBox->setStyleSheet(styleSheet());
            }
        }

        // Beginning of private functions

        void VariableEntry::setupConnections()
        {
            connect(ui->modelEntriesComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int) { emit modelSelected(); });

            connect(ui->modelEntriesComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(handleDifferentAssociatedModelSelected(const QString&)));

            connect(ui->variableSelectedCheckBox, &QCheckBox::stateChanged, [this](int state) { emit variableSelected(this, state); });

            // This will check if the new name is valid.
            connect(ui->variableNameLineEdit, &QLineEdit::textChanged, [this](const QString &newName) { emit nameChanged(this, newName); });
        }
    }
}