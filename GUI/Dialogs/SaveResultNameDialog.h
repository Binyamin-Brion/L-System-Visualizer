//
// Created by binybrion on 5/9/20.
//

#ifndef VOXEL_L_SYSTEM_SAVERESULTNAMEDIALOG_H
#define VOXEL_L_SYSTEM_SAVERESULTNAMEDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui
{
    class SaveResultNameDialog;
}

namespace GUI
{
    namespace Dialogs
    {
        /**
         * A dialog to request the user to enter a name for a bookmarked favourite execution result.
         */

        class SaveResultNameDialog : public QDialog
        {
                Q_OBJECT

            public:

                /**
                 * Initializes the window using the passed in parent.
                 *
                 * @param parent that has ownership of this widget
                 */
                explicit SaveResultNameDialog(QWidget *parent = nullptr);

                /**
                 * Adds the passed in name to the list of already taken names. After passing in the name, a new bookmarked
                 * result will not be able to have the same name until that taken name is removed.
                 *
                 * @param name that a new bookmarked result is not allowed to have
                 */
                void addExistingName(const QString &name);

                void clearExistingName();

                /**
                 * Executes the dialog.
                 *
                 * @return status return code of the dialog
                 */
                int exec() override;

                /**
                 * Get the name of the bookmarked result that the user entered the last time the dialog was executed.
                 *
                 * This function is undefined if in the previous dialog execution the user did not enter any name and
                 * cancelled the dialog.
                 *
                 * @return the most recently entered name.
                 */
                QString getMostRecentName() const;

                /**
                 * Check if any names have been entered previously.
                 *
                 * @return true if no previously existing names have been entered
                 */
                bool noExistingNames() const;

                /**
                 * Removes the existing name from the list of already taken names.
                 *
                 * This function is undefined if the passed in name was never taken.
                 *
                 * @param name to remove from the existing name lsit
                 */
                void removeExistingName(const QString &name);

            private slots:

                /**
                 * Checks if the name is unique. If it is not, then the user will not be able to proceed with the current
                 * entered name.
                 *
                 * @param name to check for uniqueness
                 */
                void checkUniqueName(const QString &name);

            private:

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                Ui::SaveResultNameDialog *ui = nullptr;
                std::vector<QString> existingNames;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SAVERESULTNAMEDIALOG_H
