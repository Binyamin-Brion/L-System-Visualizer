//
// Created by binybrion on 5/2/20.
//

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <ModelLoading/Model.h>
#include "VariableTabContent.h"
#include "ui_variableTabContent.h"

namespace GUI
{
    namespace L_System
    {
        VariableTabContent::VariableTabContent(QWidget *parent)
                            :
                                QWidget{parent},
                                ui{new Ui::VariableTabContent}
        {
            ui->setupUi(this);

            // By default there are no variables that can be chosen as an axiom.
            ui->axiomComboBox->addItem(invaliAxiomString);

           setupConnections();
        }

        void VariableTabContent::clearExistingModelEntries()
        {
            ui->declaredVariables->clearExistingModelEntries();
        }

        ::L_System::DataStructures::Variable VariableTabContent::getAxiom() const
        {
            return ::L_System::DataStructures::Variable{ui->axiomComboBox->currentText(), ui->declaredVariables->getAssociatedModelName(ui->axiomComboBox->currentText())};
        }

        std::vector<::L_System::DataStructures::Variable> VariableTabContent::getVariablesTokens() const
        {
            return ui->declaredVariables->getVariablesTokens();
        }

        void VariableTabContent::loadVariables(const ::L_System::DataStructures::Variable &axiom, const std::vector<::L_System::DataStructures::Variable> &variables)
        {
            ui->axiomComboBox->clear();

            ui->declaredVariables->loadVariables(variables);

            // Find the location of the axiom within the variable vector to show it in the axiom combo box.
            int axiomIndex = 0;
            bool foundAxiomIndex = false; // Keeps track of whether or not to increment the axiom index.

            // The axiom must be one of the variables in the vector.
            for(const auto &i : variables)
            {
                ui->axiomComboBox->addItem(i.getVariableName());

                // Don't break after finding matching variable- this prevents all of the variables from being added to
                // the axiom combo box.
                if(i.getVariableName() == axiom.getVariableName())
                {
                    foundAxiomIndex = true;
                }

                if(!foundAxiomIndex)
                {
                    axiomIndex += 1;
                }
            }

            ui->axiomComboBox->setCurrentIndex(axiomIndex);
        }

        void VariableTabContent::loadNewModel(const QString &modelFileLocation)
        {
            loadModel(modelFileLocation);
        }

        // Beginning of public slots

        void VariableTabContent::setConstantNames(const std::vector<QString> &constantNames)
        {
            ui->declaredVariables->setConstantNames(constantNames);
        }

        // Beginning of private slots

        void VariableTabContent::loadNewModel()
        {
            // Note: for some reason limiting the files that can be chosen in the file dialog based off of the file extension
            // does note work.
            QString modelFileLocation = QFileDialog::getOpenFileName(this, "Open Mode", QDir::homePath());

            loadModel(modelFileLocation);
        }

        void VariableTabContent::refreshAxiomList(std::vector<QString> optionList)
        {
            // To prevent the previous set of variable names from being shown, all of the previous set is removed.
            ui->axiomComboBox->clear();

            // Add all of the updated valid variable names to the combo box.
            for(const auto &i : optionList)
            {
                ui->axiomComboBox->addItem(i);
            }

            // No valid variable names to choose as an axiom.
            if(optionList.empty())
            {
                ui->axiomComboBox->addItem(invaliAxiomString);
            }
        }

        // Beginning of private functions

        void VariableTabContent::setupConnections()
        {
            connect(ui->loadModelButton, SIGNAL(clicked()), this, SLOT(loadNewModel()));

            connect(ui->addVariableButton, SIGNAL(clicked()), ui->declaredVariables, SLOT(addVariableEntry()));

            connect(ui->deleteVariableButtons, SIGNAL(clicked()), ui->declaredVariables, SLOT(handleDeleteButtonPushed()));

            connect(ui->declaredVariables, SIGNAL(variablesChangedValidity(std::vector<QString>)), this, SLOT(refreshAxiomList(std::vector<QString>)));

            connect(ui->declaredVariables, &VariablesWidget::variablesChangedValidity, [this](std::vector<QString> variableNames) { emit variablesChangedValidity(variableNames); });
        }

        void VariableTabContent::loadModel(const QString &modelFileLocation)
        {
            if(modelFileLocation.isEmpty())
            {
                QMessageBox::critical(this, "Load Model", "Unable to load the requested model: empty file location given.", QMessageBox::Ok);

                return;
            }

            if(!modelFileLocation.contains(".obj")) // The selected file may not actually be file containing a valid model.
            {
                int reply = QMessageBox::question(this, "Load Model", "The file you selected does not appear to an OBJ file.\n\n"
                                                                      "Proceed to load the selected model?", QMessageBox::Yes | QMessageBox::No);

                if(reply == QMessageBox::No)
                {
                    return;
                }
            }

            // If the model was successfully loaded, then emit respective signal so that the model can be uploaded into
            // GPU memory.

            try
            {
                ::ModelLoading::Model loadedModel{modelFileLocation.toStdString()};

                emit modelLoaded(loadedModel);

                ui->declaredVariables->addModelEntry(modelFileLocation);
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::critical(this, "Load Model", "Unable to load the requested model.\n\n" +
                                                          QString{e.what()}, QMessageBox::Ok);
            }
        }

        bool VariableTabContent::hasValidAxiom() const
        {
            return !ui->axiomComboBox->currentText().isEmpty();
        }
    }
}
