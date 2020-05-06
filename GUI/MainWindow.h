//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_MAINWINDOW_H
#define VOXEL_L_SYSTEM_MAINWINDOW_H

#include "ui_mainwindow.h"

class Ui_MainWindow;

namespace GUI
{
    /**
     * Main window which contains all of the GUI components.
     */

    class MainWindow : public QMainWindow
    {
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

        private:

            void createConnections();

            Ui::MainWindow *ui = nullptr;
    };
}

#endif //VOXEL_L_SYSTEM_MAINWINDOW_H
