//
// Created by binybrion on 4/30/20.
//

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent)
                :
                    QMainWindow{parent},
                    ui{new Ui::MainWindow}
    {
        ui->setupUi(this);

        setMinimumSize(1280, 720);

        setupConnections();
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

    void MainWindow::openProject()
    {
        // The filter in the file dialog does not work for some reason- no relevant files are shown for some reason.
        // Thus if a file that is not the project file is entered, then the ProjectLoader will cause an error message
        // to be shown.

        QString modelFileLocation = QFileDialog::getOpenFileName(this, "Open Mode", QDir::homePath());

        ui->scriptTabWidget->openProject(modelFileLocation);
    }

    void MainWindow::saveProject()
    {
        QString saveLocation = QFileDialog::getSaveFileName(this, "Save Project", QDir::homePath());

        // If the user did not specify the project file extension in the save dialog, then add it.
        if(!saveLocation.contains(projectFileExtension))
        {
            saveLocation += projectFileExtension;
        }

        ui->scriptTabWidget->saveProject(saveLocation);
    }

    // Beginning of private functions

    void MainWindow::setupConnections()
    {
        connect(ui->addScriptButton, SIGNAL(clicked()), ui->scriptTabWidget, SLOT(addNewTab()));

        connect(ui->renameScriptButton, SIGNAL(clicked()), ui->scriptTabWidget, SLOT(renameCurrentTab()));

        connect(ui->discardScriptButton, SIGNAL(clicked()), ui->scriptTabWidget, SLOT(removeCurrentTab()));

        connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProject()));
    }
}