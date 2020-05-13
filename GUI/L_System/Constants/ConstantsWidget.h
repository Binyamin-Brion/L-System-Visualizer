//
// Created by binybrion on 5/3/20.
//

#ifndef VOXEL_L_SYSTEM_CONSTANTSWIDGET_H
#define VOXEL_L_SYSTEM_CONSTANTSWIDGET_H

#include <QWidget>
#include "../../../L_System/DataStructures/Constants/Constant.h"

class QVBoxLayout;

namespace GUI
{
    namespace L_System
    {
        class ConstantEntryDeclaration;

        /**
         *  Holds all of the constants entries held in the Constants Tab.
         */

        class ConstantsWidget : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when an entry has its name or information changed, resulting in the constant becoming
                 * either valid or invalid.
                 */
                void constantsChangedValidity(std::vector<QString>);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ConstantsWidget(QWidget *parent = nullptr);

                /**
                 * Get the names of all of the entries that are valid.
                 *
                 * @return list of all valid entry names.
                 */
                std::vector<QString> getConstantNames() const;

                /**
                 * Get the list of Constant equivalent tokens of all of the constant entries.
                 *
                 * @return list of equivalent Constant tokens
                 */
                std::vector<::L_System::DataStructures::Constant> getConstantsTokens() const;

                /**
                 * Creates entries for each of the constant data structures held in the passed in vector.
                 *
                 * @param constants vector of constant data structure from which to create entries
                 */
                void loadEntries(const std::vector<::L_System::DataStructures::Constant> &constants);

                /**
                 * Updates the variables names that are currently being used.
                 *
                 * @param constantNames vector of valid variable names
                 */
                void setVariableNames(const std::vector<QString> &variableNames);

            public slots:

                /**
                 * Called when a new entry needs to be created. This is done by clicking the appropriate button in the
                 * this object.
                 */
                void addConstantEntry();

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
                void handleNewConstantName(ConstantEntryDeclaration *entry, const QString &newName);

                /**
                 * Called when an entry's associated checkbox has its status changed.
                 *
                 * @param variableEntry whose associated checkbox was changed
                 * @param newState the current state of the checkbox
                 */
                void handleConstantEntrySelected(ConstantEntryDeclaration *constantEntry, int newState);

            private:

                /**
                 * Determines if the passed in name is valid, meaning it is unique and does not contain whitespaces and
                 * has at least one character.
                 *
                 * @param newName to be checked for validity
                 * @return true if the passed in name is valid
                 */
                bool newConstantNameUnique(const QString &newName);

                /**
                 * Removes all current entries in the layout, and deallocate them as well.
                 */
                void removeExistingEntries();

                QVBoxLayout *layout = nullptr;

                std::vector<ConstantEntryDeclaration*> constants;
                std::vector<ConstantEntryDeclaration*> selectedConstants;

                // Keeps track of the name of each entry.
                struct EntryNames
                {
                    const ConstantEntryDeclaration* entryDeclaration;
                    QString name;
                    bool validName = false;
                };

                std::vector<EntryNames> constantNames;
                std::vector<QString> variableNames;
        };
    }
}

#endif //VOXEL_L_SYSTEM_CONSTANTSWIDGET_H
