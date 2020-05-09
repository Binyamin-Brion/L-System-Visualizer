//
// Created by binybrion on 5/2/20.
//

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include "VariablesWidget.h"
#include "VariableEntry.h"
#include <cassert>

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
            // If the model entry was already added to a combo box, then it should not appear again.
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

        // Beginning of public slots

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

        QString VariablesWidget::getAssociatedModelName(const QString &variableName) const
        {
            for(const auto &i : variableNames)
            {
                if(i.name == variableName)
                {
                    return i.entry->getAssociatedModelName();
                }
            }

            assert(false);
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

        std::vector<::L_System::DataStructures::Variable> VariablesWidget::getVariablesTokens() const
        {
            std::vector<::L_System::DataStructures::Variable> variablesTokens;

            for(auto &i : variableNames)
            {
                if(i.nameValid)
                {
                    variablesTokens.push_back(i.entry->getVariableToken());
                }
            }

            return variablesTokens;
        }

        void VariablesWidget::handleDeleteButtonPushed()
        {
            // For all of the selected entries, remove them visually, then remove them from the program.
            for(auto i : selectedVariables)
            {
                layout->removeWidget(i);

                // Call delete BEFORE erasing the variable from the variables vector (at which point the pointer to object is lost)
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

            // Automatically update the list of variables available for use in a rule.
            emit entryNamesChanged(getEntryNames());
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

        // Beginning of private slots

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

            // Automatically update the list of variables available for use in a rule.
            emit entryNamesChanged(getEntryNames());
        }

        // Beginning of private functions

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
