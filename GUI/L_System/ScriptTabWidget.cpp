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

            ui->splitter->setStretchFactor(0, 1);
            ui->splitter->setStretchFactor(1, 10);
            ui->splitter_2->setStretchFactor(1, 1);

            setupConnections();
        }

        void ScriptTabWidget::openProject(const ::ProjectSaverLoader::ScriptInformation &scriptInformation)
        {
            // Loading a project may mean loading saved bookmarked results- these should not trigger a warning asking the user to save,
            // as adding saved bookmarks to the savedResultComboBox will trigger the connections as the index of that widget will change.
            savedNewResult = true;

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
            // Ensure that the current favourite script is saved, even if user forgot to do so. This done as when changing
            // a favourite script, the user is given a warning of whether to save or not, but if the user saves the project,
            // nothing is given, thus no reminder to save the changes made to the current favourite result. Worst case
            // the user can delete the instances they added when they reopen the project.
            if(!previousFavouriteResult.isEmpty() && previousFavouriteResult != newResultEntry)
            {
                ui->scriptInfoTabs->saveFavouriteScript(previousFavouriteResult);
            }

            ui->scriptInfoTabs->saveProject(scriptName, projectDetails);
        }

        // Beginning of private slots

        void ScriptTabWidget::bookmarkCurrentScriptExecutionResult()
        {
            if(saveResultNameDialog->exec() == QDialog::Accepted)
            {
                savedNewResult = true;

                // Call this BEFORE editing the savedResultsComboBox, as doing so changes the index of that combo box, triggering the connections.
                // If the new result is not add to the tab widget first, then a segfault could occur as the connection slot would search for a saved result
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
            requestFavouriteResultSave();

            currentSaveResultIndex = index;
        }

        void ScriptTabWidget::handleChangedSaveScript(const QString &text)
        {
            // When loading a saved project, the stack combo box is cleared, causing the input to this function to be nothing.
            // To prevent a crash from occurring, this function exits if such an input is given.
            if(text.isEmpty() || text == newResultEntry)
            {
                ui->renderResultWidget->enableAddInstanceButton(false);

                return;
            }

            // The previous favourite script needs to be saved to ensure no data is lost. Since modifications made to the
            // new favourite result will overwrite the user added instance data, the save must be made now, before the user
            // makes changes there.

            // Initial copy needed here for the first time this slot is executed.
            static QString previousFavouriteResult = text;

            requestUserAddedInstancesSave();

            previousFavouriteResult = text;

            // Keep track of the previous favourite result in a member object so that it is known which favourite result to save
            // when the project is saved.
            this->previousFavouriteResult = text;

            // The user can only add instances if they are viewing a favourite result they have saved.
            ui->renderResultWidget->enableAddInstanceButton(true);

            const ::ProjectSaverLoader::FavouriteResult favouriteResult = ui->scriptInfoTabs->getFavouriteScript(text);

            // Allow user to remove the current bookmarked result as well as render it.
            ui->renderSavedResultButton->setEnabled(true);

            ui->discardResultButton->setEnabled(true);

            ui->savedResultOutputWidget->showSavedScriptOutput(favouriteResult.executionResult);
        }

        void ScriptTabWidget::removeBookmarkedScriptExecutionResult()
        {
            // The user does not have any saved favourite (bookmarked) results.
            const QString currentFavouriteSaveResult = ui->savedResultsComboBox->currentText();

            // Ensure that calling render is done only a valid bookmarked result.
            if(currentFavouriteSaveResult.isEmpty() || currentFavouriteSaveResult == newResultEntry)
            {
                return;
            }

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

            // Ensure that calling render is done only a valid bookmarked result.
            if(currentFavouriteSaveResult.isEmpty() || currentFavouriteSaveResult == newResultEntry)
            {
                ui->renderResultWidget->enableAddInstanceButton(false);

                return;
            }

            const ::ProjectSaverLoader::FavouriteResult favouriteResult = ui->scriptInfoTabs->getFavouriteScript(currentFavouriteSaveResult);

            ::L_System::Interpretation::Interpreter::interpretExistingResult(favouriteResult.executionResult);

            ui->renderResultWidget->addModelInstances();

            // With the interpretation result added to the render, now add the user added instances. This has to be after
            // due to the assumption made in the code that organizes the render data.
            ui->renderResultWidget->addUserRequestedModelInstances(favouriteResult.userDefinedInstances);
        }

        void ScriptTabWidget::requestUserAddedInstancesSave()
        {
            if(unsavedUserAddedInstances)
            {
                // Assume that the user saved, as they are given a chance to do so- if they decline, it is assumed they thought about the consequences
                // of not doing so.
                unsavedUserAddedInstances = false;

                int response = QMessageBox::warning(this, "Unsaved Added Instances", "Previously added user instances were not saved. Would you like to save them?", QMessageBox::Yes | QMessageBox::No);

                if(response == QMessageBox::Yes)
                {
                    ui->scriptInfoTabs->saveFavouriteScript(previousFavouriteResult);
                }
            }
        }

        void ScriptTabWidget::runScript()
        {
            // Before running a script, check if the previous result needs to be saved, and if it does, ask the user to do so.
            // Note: this is required because running a script discard the previous result.
            requestFavouriteResultSave();

            try
            {
                // If not all the required information is present to run a script, for whatever reason, such as an invalid axiom,
                // then don't proceed to try and execute the script. Abort the execution and show an error message.
                ui->scriptInfoTabs->setScriptInput();
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::critical(this, "Error running script", e.what(), QMessageBox::Ok);

                return;
            }

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

            ui->renderResultWidget->addModelInstances();

            ui->discardResultButton->setEnabled(false);

            ui->savedResultsComboBox->addItem(newResultEntry);

            ui->savedResultsComboBox->setCurrentIndex(ui->savedResultsComboBox->count() - 1);

            // Allow user to save, and ensure a warning is given if a user tries to look at a different bookmarked result
            // without saving the current new result first.
            savedNewResult = false;

            ui->saveResultButton->setEnabled(true);
        }

        // Beginning of private functions

        void ScriptTabWidget::requestFavouriteResultSave()
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
            connect(ui->renderResultWidget, &OpenGL::RenderResultWidget::modelInstancesChanged, [this](const std::vector<::ProjectSaverLoader::UserDefinedInstances> &instances)
            {
                ui->scriptInfoTabs->setModelInstances(instances);

                unsavedUserAddedInstances = true;
            });

            connect(ui->saveUserAddedInstancesButton, SIGNAL(clicked()), this, SLOT(requestUserAddedInstancesSave()));

            connect(ui->scriptInfoTabs, SIGNAL(modelLoaded(const ::ModelLoading::Model&)), ui->renderResultWidget, SLOT(uploadModelVRam(const ::ModelLoading::Model&)));

            connect(ui->runScriptButton, SIGNAL(clicked()), this, SLOT(runScript()));

            connect(ui->saveResultButton, SIGNAL(clicked()), this, SLOT(bookmarkCurrentScriptExecutionResult()));

            connect(ui->discardResultButton, SIGNAL(clicked()), this, SLOT(removeBookmarkedScriptExecutionResult()));

            connect(ui->savedResultsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handleChangedSaveScript(int)));

            connect(ui->savedResultsComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(handleChangedSaveScript(const QString&)));

            connect(ui->renderSavedResultButton, SIGNAL(clicked()), this, SLOT(renderScript()));
        }
    }
}