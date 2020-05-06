//
// Created by binybrion on 5/2/20.
//

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include "VariablesWidget.h"
#include "VariableEntry.h"

namespace GUI
{
    namespace L_System
    {
        VariablesWidget::VariablesWidget(QWidget *parent)
                            :
                                QWidget{parent},
                                layout{new QVBoxLayout{this}}
        {
            setLayout(layout);
        }

        void VariablesWidget::addModelEntry(const QString &modelName)
        {
            // If the model entry was already added to a combox box, then it should not appear again.
            if(std::find(modelEntries.begin(), modelEntries.end(), modelName) != modelEntries.end())
            {
                return;
            }

            // Make the model name entry available to all of the variable entries' model combo box.
            for(auto &i : variables)
            {
                i->addModelEntry(modelName);
            }

            modelEntries.push_back(modelName);
        }

        void VariablesWidget::addVariableEntry()
        {
            VariableEntry *variableEntry = new VariableEntry{this};

            connect(variableEntry, SIGNAL(variableSelected(VariableEntry*, int)), this, SLOT(handleVariableEntrySelected(VariableEntry*, int)));

            connect(variableEntry, SIGNAL(nameChanged(VariableEntry*, const QString&)),
                    this, SLOT(handleNewVariableName(VariableEntry*, const QString&)));

            variables.push_back(variableEntry);

            layout->addWidget(variableEntry);

            for(const auto &i : modelEntries)
            {
                variableEntry->addModelEntry(i);
            }

            // Associate each entry with its default name when it is created.
            variableNames.push_back(EntryNames{variableEntry, "Enter a name..."});
        }

        std::vector<QString> VariablesWidget::getEntryNames() const
        {
            // Only collect the names that are valid; if they are valid they are NOT returned.

            std::vector<QString> names;

            for(const auto &i : variableNames)
            {
                if(i.nameValid)
                {
                    names.push_back(i.name);
                }
            }

            return names;
        }

        void VariablesWidget::handleDeleteButtonPushed()
        {
            // For all of the selected entries, remove them visually, then remove them from the program.
            for(auto i : selectedVariables)
            {
                layout->removeWidget(i);

                // Call delete BEFORE erasing he variable from the variables vector.
                delete i;

                auto variableLocation = std::find(variables.begin(), variables.end(), i);

                variables.erase(variableLocation);

                auto variableNameLocation = std::find_if(variableNames.begin(), variableNames.end(), [i](const EntryNames &entry)
                {
                    return i == entry.entry;
                });

                variableNames.erase(variableNameLocation);
            }

            selectedVariables.clear();
        }

        void VariablesWidget::handleVariableEntrySelected(VariableEntry *variableEntry, int newState)
        {
            if(newState == Qt::Checked)
            {
                selectedVariables.push_back(variableEntry);
            }
            else
            {
                auto selectedVariableLocation = std::find(selectedVariables.begin(), selectedVariables.end(), variableEntry);

                selectedVariables.erase(selectedVariableLocation);
            }
        }

        void VariablesWidget::handleNewVariableName(VariableEntry *entry, const QString &newName)
        {
            auto entryLocation = std::find_if(variableNames.begin(), variableNames.end(), [entry](const EntryNames &entryNames)
            {
                return entryNames.entry == entry;
            });

            bool nameValid = newVariableNameUnique(newName);

            entry->nameValid(nameValid);
            entryLocation->name = newName;
            entryLocation->nameValid = nameValid;
        }

        bool VariablesWidget::newVariableNameUnique(const QString &newName) const
        {
            for(const auto &i : variableNames)
            {
                if(i.name == newName)
                {
                    return false;
                }
            }

            if(newName.isEmpty())
            {
                return false;
            }

            return !newName.contains(QRegExp{"\\s+"});
        }
    }
}
