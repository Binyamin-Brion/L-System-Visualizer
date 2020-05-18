//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_MAINWINDOW_H
#define VOXEL_L_SYSTEM_MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

namespace GUI
{
    namespace Dialogs
    {
        class NewNameDialog;
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
             * Warns the user about losing unsaved data, and gives them an option to save the project before closing the program.
             *
             * @param event unused
             */
            void closeEvent(QCloseEvent *event) override;

            /**
             * Handles key press events when this widget is in focus.
             *
             * @param event containing information about they key press
             */
            void keyPressEvent(QKeyEvent *event) override;

        private slots:

            /**
             * Creates a new empty project.
             */
            void newProject();

            /**
             * Requests the user to select a file from the file system, and then loads the project using that file.
             */
            void openProject();

            /**
             * Requests the user to select a location to save the project to, and the project is saved to a file at the
             * given location.
             */
            void saveAsProject();

            /**
             * Saves the project in the last specified location; if none was specified, this function calls saveAsProject().
             */
            void saveProject();

        private:

            /**
             * Sets up the connections used by this object's widgets.
             */
            void setupConnections();

            Ui::MainWindow *ui = nullptr;

            QString saveLocation;

            const QString projectFileExtension = ".L_Vis";
    };
}

#endif //VOXEL_L_SYSTEM_MAINWINDOW_H
