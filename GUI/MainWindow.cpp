//
// Created by binybrion on 4/30/20.
//

#include "MainWindow.h"
#include <QKeyEvent>

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent)
                :
                    QMainWindow{parent},
                    ui{new Ui::MainWindow}
    {
        setMinimumSize(1280, 720);

        ui->setupUi(this);
    }

    void MainWindow::keyPressEvent(QKeyEvent *event)
    {
        switch(event->key())
        {
            case Qt::Key_Escape:
                QApplication::quit();
                break;
        }
    }
}