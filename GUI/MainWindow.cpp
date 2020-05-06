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

        ui->spinBox->setMinimum(1);

        //ui->splitter_6->setStretchFactor(ui->splitter->sizes().size() - 1, 1);

        createConnections();
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

    void MainWindow::createConnections()
    {
        connect(ui->tabWidget, SIGNAL(modelLoaded(const ::ModelLoading::Model&)), ui->openGLWidget, SLOT(uploadModelGPU(const ::ModelLoading::Model&)));
    }
}