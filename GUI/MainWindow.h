//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_MAINWINDOW_H
#define VOXEL_L_SYSTEM_MAINWINDOW_H

#include <QMainWindow>
#include "ProjectSaverLoader/ProjectLoader.h"
#include "ProjectSaverLoader/ProjectSaver.h"

namespace Ui
{
    class MainWindow;
}

namespace GUI
{
    namespace Dialogs
    {
        class SaveResultNameDialog;
    }

    /**
     * Main window which contains all of the GUI components.
     */

    class MainWindow : public QMainWindow
    {
            Q_OBJECT

        public:

            /**
             * Initializes the window using the passed in parent.
             *
             * @param parent that has ownership of this widget
             */
            explicit MainWindow(QWidget *parent = nullptr);

            /**
             * Handles key press events when this widget is in focus.
             *
             * @param event containing information about they key press
             */
            void keyPressEvent(QKeyEvent *event) override;

        private slots:

            /**
             *
             */
            void bookmarkCurrentScriptExecutionResult();

            void handleChangedSaveScript(int index);

            /**
             * Opens the a project after the user specifies one using a file dialog.
             */
            void openProject();

            /**
             * Makes the request to upload the script parameters to the ScriptInput data structure, then execute and
             * interpret the script. A check for no valid rules is made- if there are none, the script is not executed.
             */
            void runScript();

            void removeBookmarkedScriptExecutionResult();

            /**
             * Saves the project to a file, allowing it to be reopened using the openProject() function.
             *
             * TODO: change hard coded save destination to one where the user chooses using a file dialog
             */
            void saveProject();

        private:

            /**
             * Sets up the connections used by this object's widgets.
             */
            void setupConnections();

            Ui::MainWindow *ui = nullptr;
            Dialogs::SaveResultNameDialog *saveResultNameDialog = nullptr;
            int currentSaveResultIndex = -1;

            ::ProjectSaverLoader::ProjectLoader projectLoader;
            ::ProjectSaverLoader::ProjectSaver projectSaver;
            ::ProjectSaverLoader::ProjectDetails projectDetails;
    };
}

#endif //VOXEL_L_SYSTEM_MAINWINDOW_H
