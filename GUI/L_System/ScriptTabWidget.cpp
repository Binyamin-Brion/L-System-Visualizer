//
// Created by binybrion on 5/12/20.
//

#include "ScriptTabWidget.h"
#include "ui_scriptTabWidget.h"
#include <QKeyEvent>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDir>
#include "L_System/ScriptInput.h"
#include "L_System/Execution/Executor.h"
#include "L_System/Interpretation/Interpreter.h"
#include "GUI/Dialogs/NewNameDialog.h"

namespace GUI
{
    namespace L_System
    {
        ScriptTabWidget::ScriptTabWidget(QWidget *parent)
                            :
                                QWidget{parent},
                                ui{new Ui::ScriptTabWidget},
                                saveResultNameDialog{new Dialogs::NewNameDialog{this}}
        {
            ui->setupUi(this);

            setupConnections();
        }

        void ScriptTabWidget::openProject(const ::ProjectSaverLoader::ScriptInformation &scriptInformation)
        {
            // Loading a project may mean loading saved bookmarked results- these should not trigger a warning asking the user to save,
            // as adding saved bookmarks to the savedResultComboBox will trigger the connections as the index of that widget will change.
            savedNewResult = true;

      //      ui->openGLWidget->clearData();

            saveResultNameDialog->clearExistingName();

            // Call this function BEFORE uploading entries to the savedResultsComboBox. This ensures that as the index
            // changes when adding a new entry, triggering the appropriate slot, a Favourite result is associated with
            // each entry name, ensuring no crash happens.
            ui->scriptInfoTabs->loadProject(scriptInformation);

            // Remove any previous shown saved results
            ui->savedResultsComboBox->clear();

            // Show an entry for each of the loaded saved results.
            for(const auto &savedResult : scriptInformation.favouriteResults)
            {
                ui->savedResultsComboBox->addItem(savedResult.resultName);

                // Let the dialog associated with adding a new bookmarked result know of the names that are taken
                // so that if a new result is added, it knows whether or not to reject the new name.
                saveResultNameDialog->addExistingName(savedResult.resultName);
            }
        }

        void ScriptTabWidget::saveProject(const QString &scriptName, ::ProjectSaverLoader::ProjectDetails &projectDetails)
        {
            ui->scriptInfoTabs->saveProject(scriptName, projectDetails);
        }

        // Beginning of private slots

        void ScriptTabWidget::bookmarkCurrentScriptExecutionResult()
        {
            if(saveResultNameDialog->exec() == QDialog::Accepted)
            {
                savedNewResult = true;

                // Call this BEFORE editing the savedResultsComboBox, as doing so changes the index of that combo box, triggering the connections.
                // If the new result is not add to the tab widget first, then a segfault could occur as the connection slot would search for a daved result
                // in the tab widget, when it is only being added after the connection has been executed.
                ui->scriptInfoTabs->addFavouriteScript(saveResultNameDialog->getMostRecentName(), ::L_System::Execution::Executor::getRecursionResult());

                ui->savedResultsComboBox->removeItem(ui->savedResultsComboBox->count() - 1);

                ui->savedResultsComboBox->addItem(saveResultNameDialog->getMostRecentName());

                ui->discardResultButton->setEnabled(true);

                ui->savedResultsComboBox->setCurrentIndex(ui->savedResultsComboBox->count() - 1);
            }
        }

        void ScriptTabWidget::handleChangedSaveScript(int index)
        {
            // Before changing a viewed script, check if the previous result needs to be saved, and if it does, ask the user to do so.
            // Note: this is required because changing the viewed result discard the previous result.
            requestResultSave();

            currentSaveResultIndex = index;
        }

        void ScriptTabWidget::handleChangedSaveScript(const QString &text)
        {
            // When loading a saved project, the stack combo box is cleared, causing the input to this function to be nothing.
            // To prevent a crash from occurring, this function exits if such an input is given.
            if(text.isEmpty() || text == newResultEntry)
            {
                return;
            }

            const ::ProjectSaverLoader::FavouriteResult favouriteResult = ui->scriptInfoTabs->getFavouriteScript(text);

            // Allow user to remove the current bookmarked result as well as render it.
            ui->renderSavedResultButton->setEnabled(true);

            ui->discardResultButton->setEnabled(true);

            ui->savedResultOutputWidget->showSavedScriptOutput(favouriteResult.executionResult);
        }

        void ScriptTabWidget::removeBookmarkedScriptExecutionResult()
        {
            ui->scriptInfoTabs->removeFavouriteScript(ui->savedResultsComboBox->currentText());

            saveResultNameDialog->removeExistingName(ui->savedResultsComboBox->currentText());

            ui->savedResultsComboBox->removeItem(currentSaveResultIndex);

            if(saveResultNameDialog->noExistingNames())
            {
                ui->discardResultButton->setEnabled(false);

                ui->renderSavedResultButton->setEnabled(false);

                // Don't allow a result to be saved if all of them were removed- the user must rerun a script to enable saving again.
                ui->saveResultButton->setEnabled(false);

                // Ensure that any previous bookmarked results are not shown if all of them were removed.
                ui->savedResultOutputWidget->removePreviousResult();
            }
        }

        void ScriptTabWidget::renderScript()
        {
            // Get the execution tokens of a bookmarked result, interpret it and then render that interpretation.

            const QString currentFavouriteSaveResult = ui->savedResultsComboBox->currentText();

            const ::ProjectSaverLoader::FavouriteResult favouriteResult = ui->scriptInfoTabs->getFavouriteScript(currentFavouriteSaveResult);

            ::L_System::Interpretation::Interpreter::interpretExistingResult(favouriteResult.executionResult);

            ui->openGLWidget->addModelInstances();
        }

        void ScriptTabWidget::runScript()
        {
            // Before running a script, check if the previous result needs to be saved, and if it does, ask the user to do so.
            // Note: this is required because running a script discard the previous result.
           requestResultSave();

            ui->scriptInfoTabs->setScriptInput();

            // There must be rules to run the script. This implicitly also checks that there are valid variables and (maybe) constants
            // defined as these must exist in order for a rule to be created. *Constants are not required to create a rule.
            if(::L_System::ScriptInput::getRules().empty())
            {
                QMessageBox::critical(this, "Error running script", "No rules were specified. The script will not be executed.", QMessageBox::Ok);

                return;
            }

            ::L_System::Execution::Executor::execute(ui->runDepthSpinBox->value());

            ::L_System::Interpretation::Interpreter::interpret();

            ui->savedResultOutputWidget->showScriptOutput();

            ui->openGLWidget->addModelInstances();

            ui->discardResultButton->setEnabled(false);

            ui->savedResultsComboBox->addItem(newResultEntry);

            ui->savedResultsComboBox->setCurrentIndex(ui->savedResultsComboBox->count() - 1);

            // Allow user to save, and ensure a warning is given if a user tries to look at a different bookmarked result
            // without saving the current new result first.
            savedNewResult = false;

            ui->saveResultButton->setEnabled(true);
        }

        // Beginning of private functions

        void ScriptTabWidget::requestResultSave()
        {
            if(!savedNewResult)
            {
                // Assume that the user has saved. If they don't, such as by cancelling the save dialog, then it is assumed
                // they changed their mind and did not want to change.
                savedNewResult = true;

                ui->savedResultsComboBox->removeItem(ui->savedResultsComboBox->count() - 1);

                int response = QMessageBox::warning(this, "Unsaved Execution Result", "The previous execution was not saved. Would you like to save it?", QMessageBox::Yes | QMessageBox::No);

                if(response == QMessageBox::Yes)
                {
                    bookmarkCurrentScriptExecutionResult();
                }
            }
        }

        void ScriptTabWidget::setupConnections()
        {
            connect(ui->scriptInfoTabs, SIGNAL(modelLoaded(const ::ModelLoading::Model&)), ui->openGLWidget, SLOT(uploadModelGPU(const ::ModelLoading::Model&)));

            connect(ui->runScriptButton, SIGNAL(clicked()), this, SLOT(runScript()));

            connect(ui->saveResultButton, SIGNAL(clicked()), this, SLOT(bookmarkCurrentScriptExecutionResult()));

            connect(ui->discardResultButton, SIGNAL(clicked()), this, SLOT(removeBookmarkedScriptExecutionResult()));

            connect(ui->savedResultsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleChangedSaveScript(int)));

            connect(ui->savedResultsComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(handleChangedSaveScript(const QString&)));

            connect(ui->renderSavedResultButton, SIGNAL(clicked()), this, SLOT(renderScript()));
        }
    }
}