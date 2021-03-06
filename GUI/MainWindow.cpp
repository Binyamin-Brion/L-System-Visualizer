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

        setWindowTitle("L - System Visualizer");

        setMinimumSize(1280, 720);

        setupConnections();

    #ifdef WIN32

        // The default font when run on windows is small and hard to read. Therefore a different font is set for
        // the entire application.
        qApp->setFont(QFont{"Arial", 10});

    #endif

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

    // Beginning of private slots

    void MainWindow::handleUnexpectedSaveRequest()
    {
        saveProject();

        // The project was saved by this point, so warning the user of losing any unsaved changes in the newProject
        // could cause confusion, as the failure message displayed before this slot is called stated the project would be saved.
        newProject(true);
    }

    void MainWindow::newProject(bool ignoreWarningMessage)
    {
        if(!ignoreWarningMessage)
        {
            // User must save their changes before creating a new project, or lose any unsaved changes. Thus give them a reminder
            // to save the project if they wish, before proceeding with creating a new project.
            int response = QMessageBox::information(this, "Discard Any Unsaved Changes", "Any unsaved changes will be lost when creating a new project.\n\nContinue?",
                                                    QMessageBox::Yes | QMessageBox::No);

            if(response == QMessageBox::Yes)
            {
                return;
            }
        }

        delete ui;

        ui = new Ui::MainWindow;

        ui->setupUi(this);

        saveLocation.clear();

        // As the previous UI was deleted and the new ui does not have any connections set up
        setupConnections();
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

        connect(ui->actionNew, &QAction::triggered, [this]() { newProject(false); });

        connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProject()));

        connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsProject()));

        connect(ui->scriptTabWidget, SIGNAL(requestProjectSave()), this, SLOT(handleUnexpectedSaveRequest()));
    }
}