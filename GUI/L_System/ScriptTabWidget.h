//
// Created by binybrion on 5/12/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTTABWIDGET_H
#define VOXEL_L_SYSTEM_SCRIPTTABWIDGET_H

#include <QtWidgets/QWidget>
#include "ProjectSaverLoader/ProjectLoader.h"
#include "ProjectSaverLoader/ProjectSaver.h"

namespace Ui
{
    class ScriptTabWidget;
}

namespace GUI
{
    namespace Dialogs
    {
        class NewNameDialog;
    }

    namespace L_System
    {
        /**
         * Holds all of the widgets associated with a script, including the tabs showing the script's
         * contents, execution result and rendering result.
         */

        class ScriptTabWidget : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when the Render portion of the program experienced an error loading a model and the current
                 * project needs to be saved before creating a new project.
                 */
                void requestProjectSave();

            public:
                explicit ScriptTabWidget(QWidget *parent = nullptr);

                /**
                 * Opens the a project after the user specifies one using a file dialog.
                 */
                void openProject(const ::ProjectSaverLoader::ScriptInformation &scriptInformation);

                /**
                 * Saves the project to a file, allowing it to be reopened using the openProject() function.
                 */
                void saveProject(const QString &scriptName, ::ProjectSaverLoader::ProjectDetails &projectDetails);

            private slots:

                /**
                 * Saves the current execution result as a Favourite Result.
                 */
                void bookmarkCurrentScriptExecutionResult();

                /**
                 * Updates the current index signifying the current index of the result stack combo box.
                 *
                 * @param index the current index of the result combo box
                 */
                void handleChangedSaveScript(int index);

                /**
                 * Shows the saved result associated with the passed in name.
                 *
                 * @param text the name of the selected saved result
                 */
                void handleChangedSaveScript(const QString &text);

                /**
                 * Renders the current opened favourite result, if there is one currently opened.
                 */
                void renderScript();

                /**
                 * Removes the currently selected saved script result.
                 */
                void removeBookmarkedScriptExecutionResult();

                /**
                 *  Prompts the user to save any unsaved user added instances, if there are any.
                 */
                void requestUserAddedInstancesSave();

                /**
                 * Makes the request to upload the script parameters to the ScriptInput data structure, then execute and
                 * interpret the script. A check for no valid rules is made- if there are none, the script is not executed.
                 */
                void runScript();

            private:

                /**
                 * Prompts the user to save the current execution result if it is unsaved.
                 */
                void requestFavouriteResultSave();

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                int currentSaveResultIndex = -1;

                const QString newResultEntry = "Unsaved result";
                QString previousFavouriteResult; // Keep track of the previous
                bool savedNewResult = true;

                bool unsavedUserAddedInstances = false;

                Ui::ScriptTabWidget *ui = nullptr;
                Dialogs::NewNameDialog *saveResultNameDialog = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTTABWIDGET_H
