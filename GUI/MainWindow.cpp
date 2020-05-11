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

            ui->tabWidget->addFavouriteScript(saveResultNameDialog->getMostRecentName(), ::L_System::Execution::Executor::getRecursionResult());
        }
    }

    void MainWindow::handleChangedSaveScript(int index)
    {
        currentSaveResultIndex = index;
    }

    void MainWindow::handleChangedSaveScript(const QString &text)
    {
        // When loading a saved project, the stack combo box is cleared, causing the input to this function to be nothing.
        // To prevent a crash from occurring, this function exits if such an input is given.
        if(text.isEmpty())
        {
            return;
        }

        const ::ProjectSaverLoader::FavouriteResult favouriteResult = ui->tabWidget->getFavouriteScript(text);

        ui->scrollAreaWidgetContents->showSavedScriptOutput(favouriteResult.executionResult);
    }

    void MainWindow::openProject()
    {
        QString modelFileLocation = QFileDialog::getOpenFileName(this, "Open Mode", QDir::homePath());

        try
        {
            projectDetails = projectLoader.loadProject(modelFileLocation);

            // Call this function BEFORE uploading entries to the savedResultsComboBox. This ensures that as the index
            // changes when adding a new entry, triggering the appropriate slot, a Favourite result is associated with
            // each entry name, ensuring no crash happens.
            ui->tabWidget->loadProject(projectDetails.getScripts()[0]);

            // Remove any previous shown saved results
            ui->savedResultsComboBox->clear();

            // Show an entry for each of the loaded saved results.
            for(const auto &savedResult : projectDetails.getScripts()[0].favouriteResults)
            {
                ui->savedResultsComboBox->addItem(savedResult.resultName);
            }
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
        ui->tabWidget->removeFavouriteScript(ui->savedResultsComboBox->currentText());

        saveResultNameDialog->removeExistingName(ui->savedResultsComboBox->currentText());

        ui->savedResultsComboBox->removeItem(currentSaveResultIndex);

        if(saveResultNameDialog->noExistingNames())
        {
            ui->discardResultButton->setEnabled(false);
        }
    }

    void MainWindow::saveProject()
    {
        ::ProjectSaverLoader::ProjectDetails newProjectDetails;

        ui->tabWidget->saveProject(newProjectDetails);

        projectDetails = newProjectDetails;

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

        connect(ui->savedResultsComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(handleChangedSaveScript(const QString&)));

        connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

        connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveProject()));
    }
}