//
// Created by binybrion on 4/30/20.
//

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>
#include "../L_System/ScriptInput.h"
#include "../L_System/Execution/Executor.h"
#include "../L_System/Interpretation/Interpreter.h"
#include "Dialogs/SaveResultNameDialog.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent)
                :
                    QMainWindow{parent},
                    ui{new Ui::MainWindow},
                    saveResultNameDialog{new Dialogs::SaveResultNameDialog{this}}
    {
        setMinimumSize(1280, 720);

        ui->setupUi(this);

        ui->discardResultButton->setEnabled(false);

//        ui->spinBox->setMinimum(1);
//
//        ui->splitter_2->setStretchFactor(ui->splitter_2->sizes().size() - 1, 1);
//
//        ui->splitter_3->setStretchFactor(ui->splitter_3->sizes().size() - 1, 1);
//
//        ui->splitter_4->setStretchFactor(ui->splitter_4->sizes().size() - 1, 1);
//
//        ui->splitter_5->setStretchFactor(ui->splitter_5->sizes().size() - 1, 1);
//
//        ui->splitter_6->setStretchFactor(ui->splitter_6->sizes().size() - 1, 1);

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

    // Beginning of private slots

    void MainWindow::bookmarkCurrentScriptExecutionResult()
    {
        if(saveResultNameDialog->exec() == QDialog::Accepted)
        {
            ui->savedResultsComboBox->addItem(saveResultNameDialog->getMostRecentName());

            ui->discardResultButton->setEnabled(true);
        }
    }

    void MainWindow::handleChangedSaveScript(int index)
    {
        currentSaveResultIndex = index;
    }

    void MainWindow::openProject()
    {
        QString modelFileLocation = QFileDialog::getOpenFileName(this, "Open Mode", QDir::homePath());

        try
        {
            projectDetails = projectLoader.loadProject(modelFileLocation);

            ui->tabWidget->loadProject(projectDetails.getScripts()[0]);
        }
        catch(std::runtime_error &e)
        {
            QMessageBox::critical(this, "Error loading project.", "Unable to load the project file. Error message: \n\n" + QString{e.what()}, QMessageBox::Ok);

            return;
        }
    }

    void MainWindow::runScript()
    {
        ui->tabWidget->setScriptInput();

        // There must be rules to run the script. This implicitly also checks that there are valid variables and (maybe) constants
        // defined as these must exist in order for a rule to be created. *Constants are not required to create a rule.
        if(::L_System::ScriptInput::getRules().empty())
        {
            QMessageBox::critical(this, "Error running script", "No rules were specified. The script will not be executed.", QMessageBox::Ok);

            return;
        }

        ::L_System::Execution::Executor::execute(ui->runDepthSpinBox->value());

        ::L_System::Interpretation::Interpreter::interpret();

        ui->scrollAreaWidgetContents->showScriptOutput();

        ui->openGLWidget->addModelInstances();

        ui->discardResultButton->setEnabled(false);

        saveProject();
    }

    void MainWindow::removeBookmarkedScriptExecutionResult()
    {
        saveResultNameDialog->removeExistingName(ui->savedResultsComboBox->currentText());

        ui->savedResultsComboBox->removeItem(currentSaveResultIndex);

        if(saveResultNameDialog->noExistingNames())
        {
            ui->discardResultButton->setEnabled(false);
        }
    }

    void MainWindow::saveProject()
    {
        ui->tabWidget->saveProject(projectDetails);

        projectSaver.saveProject("/home/binybrion/saveTest.txt", projectDetails);
    }

    // Beginning of private functions

    void MainWindow::setupConnections()
    {
        connect(ui->tabWidget, SIGNAL(modelLoaded(const ::ModelLoading::Model&)), ui->openGLWidget, SLOT(uploadModelGPU(const ::ModelLoading::Model&)));

        connect(ui->runScriptButton, SIGNAL(clicked()), this, SLOT(runScript()));

        connect(ui->saveResultButton, SIGNAL(clicked()), this, SLOT(bookmarkCurrentScriptExecutionResult()));

        connect(ui->discardResultButton, SIGNAL(clicked()), this, SLOT(removeBookmarkedScriptExecutionResult()));

        connect(ui->savedResultsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleChangedSaveScript(int)));

        connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProject()));
    }
}