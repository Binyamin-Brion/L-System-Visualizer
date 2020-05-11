//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_VARIABLESWIDGET_H
#define VOXEL_L_SYSTEM_VARIABLESWIDGET_H

#include <QtWidgets/QWidget>
#include "../../../L_System/DataStructures/Variable/Variable.h"

class QVBoxLayout;

namespace GUI
{
    namespace L_System
    {
        class VariableEntry;

        /**
         *  Holds all of the variable entries held in the Variables Tab.
         */

        class VariablesWidget : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when an entry name changes, and the list of variables that can be used in a rule has to be updated.
                 */
                void entryNamesChanged(std::vector<QString>);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit VariablesWidget(QWidget *parent = nullptr);

                /**
                * Called when a model was loaded, and a variable can now represent that loaded model.
                * Forwards all the argument to all of the entries.
                *
                * @param modelName file location of the model that was loaded
                */
                void addModelEntry(const QString &modelName);

                /**
                 * Clears existing model entries that are currently stored.
                 */
                void clearExistingModelEntries();

                /**
                 * Gets the associated model name for the variable with the passed in name.
                 *
                 * @param variableName that contains the required associatedModelName
                 * @return the associatedModelName for the referred to variable
                 */
                QString getAssociatedModelName(const QString &variableName) const;

                /**
                 * Get the names of all of the entries that are valid.
                 *
                 * @return list of all valid entry names.
                 */
                std::vector<QString> getEntryNames() const;

                /**
                 * Get the list of equivalent Variable data structures for the script execution for all of the variable entries.
                 *
                 * @return list of equivalent Variable data structures
                 */
                std::vector<::L_System::DataStructures::Variable> getVariablesTokens() const;

                /**
                 * Creates entries for each of the variable data structures held in the passed in vector.
                 *
                 * @param constants vector of variable data structure from which to create entries
                 */
                void loadVariables(const std::vector<::L_System::DataStructures::Variable> &variables);

            public slots:

                /**
                 *  Called when the button to add a variable entry is clicked.
                 */
                void addVariableEntry();

                /**
                 * Called when the button to delete all of the selected variable entries is clicked.
                 */
                void handleDeleteButtonPushed();

            private slots:

                /**
                 * Determine if the new name for an entry is valid, and depending on whether it is or not, the changed
                 * entry is modified visually to represent the state of the new entry name.
                 *
                 * @param entry that had its name changed
                 * @param newName new name of the entry
                 */
                void handleNewVariableName(VariableEntry* entry, const QString &newName);

                /**
                 * Called when an entry's associated checkbox has its status changed.
                 *
                 * @param variableEntry whose associated checkbox was changed
                 * @param newState the current state of the checkbox
                 */
                void handleVariableEntrySelected(VariableEntry *variableEntry, int newState);

            private:

                /**
                 * Determines if the passed in name is valid, meaning it is unique and does not contain whitespaces and
                 * has at least one character.
                 *
                 * @param newName to be checked for validity
                 * @return true if the passed in name is valid
                 */
                bool newVariableNameUnique(const QString &newName) const;

                /**
                 * Removes all current entries in the layout, and deallocate them as well.
                 */
                void removeExistingEntries();

                std::vector<VariableEntry*> variables;
                std::vector<VariableEntry*> selectedVariables;
                QVBoxLayout *layout = nullptr;
                std::vector<QString> modelEntries;

                // Keeps track of the name of each entry.
                struct EntryNames
                {
                    const VariableEntry* entry;
                    QString name;
                    bool nameValid = false;
                };

                std::vector<EntryNames> variableNames;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLESWIDGET_H
