//
// Created by binybrion on 4/30/20.
//

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

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

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        int response = QMessageBox::information(this, "Reminder to Save Project", "Any unsaved changes will be lost by closing application.\n\n"
                                                                                  "Would you like to save the project before quitting?", QMessageBox::Yes | QMessageBox::No);

        if(response == QMessageBox::Yes)
        {
            saveProject();
        }

        QWidget::closeEvent(event);
    }

    void MainWindow::keyPressEvent(QKeyEvent *event)
    {
        switch(event->key())
        {
            case Qt::Key_Alt:

                ui->menubar->setVisible(!ui->menubar->isVisible());

                break;
        }
    }

    void MainWindow::newProject()
    {
        if(saveLocation.isEmpty())
        {
            int response = QMessageBox::information(this, "Discard Any Unsaved Changes", "Any unsaved changes will be lost when creating a new project.\n\nContinue?",
                    QMessageBox::Yes | QMessageBox::No);

            if(response == QMessageBox::Yes)
            {
                delete ui;

                ui = new Ui::MainWindow;

                ui->setupUi(this);

                saveLocation.clear();
            }
        }
    }

    void MainWindow::openProject()
    {
        // The filter in the file dialog does not work for some reason- no relevant files are shown for some reason.
        // Thus if a file that is not the project file is entered, then the ProjectLoader will cause an error message
        // to be shown.

        QString projectFileLocation = QFileDialog::getOpenFileName(this, "Open Mode", QDir::homePath());

        ui->scriptTabWidget->openProject(projectFileLocation);

        saveLocation = projectFileLocation;
    }

    void MainWindow::saveAsProject()
    {
        saveLocation = QFileDialog::getSaveFileName(this, "Save Project", QDir::homePath());

        // If the user did not specify the project file extension in the save dialog, then add it.
        if(!saveLocation.contains(projectFileExtension))
        {
            saveLocation += projectFileExtension;
        }

        if(QFile::exists(saveLocation))
        {
            int response = QMessageBox::warning(this, "Writing to Existing File", "The file " + saveLocation + " already exists.\n\nOverwrite it?", QMessageBox::Yes | QMessageBox::No);

            if(response != QMessageBox::Yes)
            {
                return;
            }
        }

        ui->scriptTabWidget->saveProject(saveLocation);
    }

    void MainWindow::saveProject()
    {
        if(saveLocation.isEmpty())
        {
            saveAsProject();

            return;
        }

        ui->scriptTabWidget->saveProject(saveLocation);
    }

    // Beginning of private functions

    void MainWindow::setupConnections()
    {
        connect(ui->addScriptButton, SIGNAL(clicked()), ui->scriptTabWidget, SLOT(addNewTab()));

        connect(ui->renameScriptButton, SIGNAL(clicked()), ui->scriptTabWidget, SLOT(renameCurrentTab()));

        connect(ui->discardScriptButton, SIGNAL(clicked()), ui->scriptTabWidget, SLOT(removeCurrentTab()));

        connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newProject()));

        connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProject()));

        connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsProject()));
    }
}