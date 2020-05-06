//
// Created by binybrion on 5/5/20.
//

#include "AxiomTabContent.h"
#include "ui_axiomTabContent.h"

namespace GUI
{
    namespace L_System
    {
        AxiomTabContent::AxiomTabContent(QWidget *parent)
                            :
                                QWidget{parent},
                                ui{new Ui::AxiomTabContent}
        {
            ui->setupUi(this);

            setupConnections();
        }

        void AxiomTabContent::addVariable(const std::vector<QString> &variableNames)
        {
            // To prevent the previous set of variable names from being shown, all of the previous set is removed.
            ui->axiomComboxBox->clear();

            // Add all of the updated valid variable names to the combo box.
            for(const auto &i : variableNames)
            {
                ui->axiomComboxBox->addItem(i);
            }

            storedVariableNames = variableNames;
        }

        void AxiomTabContent::removeVariables(const std::vector<QString> &variableNames)
        {
            // Find where each variable name is located relative to the beginning of the list stored in the combox box,
            // and use that location to remove it both graphically and internally. Remember that the storedVariable
            // holds names in the same order as appears graphically.
            for(const auto &i : variableNames)
            {
                auto variableLocation = std::find(storedVariableNames.begin(), storedVariableNames.end(), i);

                unsigned int comboBoxIndex = std::distance(storedVariableNames.begin(), variableLocation);

                ui->axiomComboxBox->removeItem(comboBoxIndex);

                storedVariableNames.erase(variableLocation);
            }
        }

        void AxiomTabContent::setupConnections()
        {
            // Ask for an updated list of valid variable names
            connect(ui->refreshButton, &QPushButton::clicked, [this]() { emit refreshButtonClicked(); });
        }
    }
}