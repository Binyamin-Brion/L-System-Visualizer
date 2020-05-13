//
// Created by binybrion on 5/3/20.
//

#include "ConstantsWidget.h"
#include <QVBoxLayout>

#include "ConstantEntryDeclaration.h"

namespace GUI
{
    namespace L_System
    {
        ConstantsWidget::ConstantsWidget(QWidget *parent)
                            :
                                QWidget{parent},
                                layout{new QVBoxLayout{this}}
        {
            setLayout(layout);
        }

        std::vector<QString> ConstantsWidget::getConstantNames() const
        {
            // Only collect the names that are valid; if they are valid they are NOT returned.

            std::vector<QString> names;

            for(const auto &i : constantNames)
            {
                if(i.validName && i.entryDeclaration->informationValid())
                {
                    names.push_back(i.name);
                }
            }

            return names;
        }

        std::vector<::L_System::DataStructures::Constant> ConstantsWidget::getConstantsTokens() const
        {
            std::vector<::L_System::DataStructures::Constant> constantsTokens;

            for(const auto &i : constantNames)
            {
                if(i.validName)
                {
                    constantsTokens.push_back(i.entryDeclaration->getConstantToken());
                }
            }

            return constantsTokens;
        }

        void ConstantsWidget::loadEntries(const std::vector<::L_System::DataStructures::Constant> &constants)
        {
            removeExistingEntries();

            for(const auto &i : constants)
            {
                ConstantEntryDeclaration *constantEntry = new ConstantEntryDeclaration{i, this};

                layout->addWidget(constantEntry);

                connect(constantEntry, SIGNAL(constantEntrySelected(ConstantEntryDeclaration*, int)), this, SLOT(handleConstantEntrySelected(ConstantEntryDeclaration*, int)));

                connect(constantEntry, SIGNAL(nameChanged(ConstantEntryDeclaration*, const QString&)),this, SLOT(handleNewConstantName(ConstantEntryDeclaration*, const QString&)));

                connect(constantEntry, &ConstantEntryDeclaration::informationChanged, [this]() { emit constantsChangedValidity(getConstantNames()); });

                this->constants.push_back(constantEntry);

                // Associate each entry with its default name when it is created.
                constantNames.push_back(EntryNames{constantEntry, i.getConstantName()});
                constantNames.back().validName = true;
            }
        }

        void ConstantsWidget::setVariableNames(const std::vector<QString> &variableNames)
        {
            this->variableNames = variableNames;
        }

        // Beginning of public slots

        void ConstantsWidget::addConstantEntry()
        {
            ConstantEntryDeclaration *constantEntry = new ConstantEntryDeclaration{this};

            layout->addWidget(constantEntry);

            connect(constantEntry, SIGNAL(constantEntrySelected(ConstantEntryDeclaration*, int)), this, SLOT(handleConstantEntrySelected(ConstantEntryDeclaration*, int)));

            connect(constantEntry, SIGNAL(nameChanged(ConstantEntryDeclaration*, const QString&)), this, SLOT(handleNewConstantName(ConstantEntryDeclaration*, const QString&)));

            connect(constantEntry, &ConstantEntryDeclaration::informationChanged, [this]() { emit constantsChangedValidity(getConstantNames()); });

            constants.push_back(constantEntry);

            // Associate each entry with its default name when it is created.
            constantNames.push_back(EntryNames{constantEntry, "Enter a name..."});
        }

        void ConstantsWidget::handleDeleteButtonPushed()
        {
            // For all of the selected entries, remove them visually, then remove them from the program.
            for(auto i : selectedConstants)
            {
                layout->removeWidget(i);

                // Call delete BEFORE erasing he variable from the variables vector.
                delete i;

                auto variableLocation = std::find(constants.begin(), constants.end(), i);

                constants.erase(variableLocation);

                auto variableNameLocation = std::find_if(constantNames.begin(), constantNames.end(), [i](const EntryNames &entry)
                {
                    return i == entry.entryDeclaration;
                });

                constantNames.erase(variableNameLocation);
            }

            selectedConstants.clear();

            // Automatically update the list of constants available for use in a rule.
            emit constantsChangedValidity(getConstantNames());
        }

        // Beginning of private slots

        void ConstantsWidget::handleNewConstantName(ConstantEntryDeclaration *entry, const QString &newName)
        {
            auto entryLocation = std::find_if(constantNames.begin(), constantNames.end(), [entry](const EntryNames &entryNames)
            {
                 return entryNames.entryDeclaration == entry;
            });

            bool nameValid = newConstantNameUnique(newName);

            // Visually update the look of the line edit holding the entry name depending on if the entry name is valid.
            entry->nameValid(nameValid);

            // Keep track of the new name of the new entry.
            entryLocation->name = newName;
            entryLocation->validName = nameValid;

            // Automatically update the list of constants available for use in a rule.
            emit constantsChangedValidity(getConstantNames());
        }

        void ConstantsWidget::handleConstantEntrySelected(ConstantEntryDeclaration *constantEntry, int newState)
        {
            if(newState == Qt::Checked)
            {
                selectedConstants.push_back(constantEntry);
            }
            else
            {
                auto selectedVariableLocation = std::find(selectedConstants.begin(), selectedConstants.end(), constantEntry);

                selectedConstants.erase(selectedVariableLocation);
            }
        }

        // Beginning of private functions

        bool ConstantsWidget::newConstantNameUnique(const QString &newName)
        {
            // Check for uniqueness.
            for(const auto &i : constantNames)
            {
                if(i.name == newName)
                {
                    return false;
                }
            }

            // Constant names cannot be the same as a variable.
            for(const auto &i : variableNames)
            {
                if(i == newName)
                {
                    return false;
                }
            }

            // Check that the name is not empty.
            if(newName.isEmpty())
            {
                return false;
            }

            // Check for no whitespaces.
            return !newName.contains(QRegExp{"\\s+"});
        }

        void ConstantsWidget::removeExistingEntries()
        {
            QLayoutItem *layoutItem = nullptr;

            while((layoutItem = layout->takeAt(0)) != nullptr)
            {
                delete layoutItem->widget();
                delete layoutItem;
            }

            this->constants.clear();

            constantNames.clear();
        }
    }
}