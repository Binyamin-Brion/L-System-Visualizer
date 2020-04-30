//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_MAINWINDOW_H
#define VOXEL_L_SYSTEM_MAINWINDOW_H

#include "ui_mainwindow.h"

class Ui_MainWindow;

namespace GUI
{
    class MainWindow : public QMainWindow
    {
        public:
            explicit MainWindow(QWidget *parent = nullptr);
            void keyPressEvent(QKeyEvent *event) override;

        private:

            Ui::MainWindow *ui = nullptr;
    };
}

#endif //VOXEL_L_SYSTEM_MAINWINDOW_H
