//
// Created by binybrion on 5/12/20.
//

#include <QtWidgets/QMessageBox>
#include "ScriptTabs.h"
#include "ScriptTabWidget.h"
#include "../Dialogs/NewNameDialog.h"

namespace GUI
{
    namespace L_System
    {
        ScriptTabs::ScriptTabs(QWidget *parent)
                    :
                        QTabWidget{parent},
                        newTabNameDialog{new Dialogs::NewNameDialog{this}}
        {
            addDefaultTab();

            newTabNameDialog->addExistingName(defaultTabName);
        }

        void ScriptTabs::addDefaultTab()
        {
            if(findTabName(defaultTabName) == tabs.end())
            {
                addNewTab(defaultTabName);
            }
        }

        void ScriptTabs::openProject(const QString &modelFileLocation)
        {
            removeExistingTabs();

            try
            {
                projectDetails = projectLoader.loadProject(modelFileLocation);

                for(const auto &script : projectDetails.getScripts())
                {
                    addNewTab(script.scriptName);

                    tabs.back().scriptTabWidget->openProject(script);
                }
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::critical(this, "Error loading project.", "Unable to load the project file. Error message: \n\n" + QString{e.what()}, QMessageBox::Ok);

                addDefaultTab();

                return;
            }
        }

        void ScriptTabs::saveProject(const QString &modelFileLocation)
        {
            ::ProjectSaverLoader::ProjectDetails newProjectDetails;

            for(auto &i : tabs)
            {
                i.scriptTabWidget->saveProject(i.name, newProjectDetails);
            }

            projectDetails = newProjectDetails;

            projectSaver.saveProject(modelFileLocation, projectDetails);
        }

        // Beginning of public slots

        void ScriptTabs::addNewTab()
        {
            if(newTabNameDialog->exec() == QDialog::Accepted)
            {
                addNewTab(newTabNameDialog->getMostRecentName());
            }
        }

        void ScriptTabs::removeCurrentTab()
        {
            int userResponse = QMessageBox::warning(this, "Confirmation Required",
                                                    "After discarding this script, it will be lost permanently. This action cannot be undone.\n\nContinue?",
                                                    QMessageBox::Yes | QMessageBox::No);

            if(userResponse == QMessageBox::Yes)
            {
                // After removing the current tab, the index may change. However, the currentIndex still specifies where
                // in the tabs vector to remove a Tab element. Thus a copy before it changes is required.
                int currentTabIndex = currentIndex();

                // Call removeTab() before deleting it; otherwise two tabs are deleted instead of one.
                // Could just remove the removeTab() call entirely, but at the time of writing, seems to logically make
                // more sense to remove the tab and then delete it.
                removeTab(currentTabIndex);

                delete tabs[currentTabIndex].scriptTabWidget;

                tabs.erase(tabs.begin() + currentTabIndex);

                if(tabs.empty())
                {
                    addDefaultTab();
                }
            }
        }

        void ScriptTabs::renameCurrentTab()
        {
            if(newTabNameDialog->exec() == QDialog::Accepted)
            {
                // Before the old name is lost, make sure to free it from the list of existing names.
                newTabNameDialog->removeExistingName(tabs[currentIndex()].name);

                // Now the name can be changed.
                tabs[currentIndex()].name = newTabNameDialog->getMostRecentName();

                // Keep track of the index that the tab was in, as after removing the tab is may change.
                int currentTabIndex = currentIndex();

                removeTab(currentTabIndex);

                // Add the renamed tab in the same place as it was before.
                addNewTab(newTabNameDialog->getMostRecentName(), currentTabIndex);

                setCurrentIndex(currentTabIndex);
            }
        }

        // Beginning of private functions

        void ScriptTabs::addNewTab(const QString &tabName, int position)
        {
            ScriptTabWidget *scriptTabWidget = new ScriptTabWidget{this};

            if(position < 0)
            {
                tabs.emplace_back(Tab{tabName, scriptTabWidget});

                addTab(scriptTabWidget, tabName);
            }
            else
            {
                tabs.insert(tabs.begin() + position, Tab{tabName, scriptTabWidget});

                insertTab(position, scriptTabWidget, tabName);
            }
        }

        std::vector<ScriptTabs::Tab>::const_iterator ScriptTabs::findTabName(const QString &tabName) const
        {
            return std::find_if(tabs.begin(), tabs.end(), [&tabName](const Tab &tab)
            {
                return tab.name == tabName;
            });
        }

        void ScriptTabs::removeExistingTabs()
        {
            clear();

            for(const auto &i : tabs)
            {
                delete i.scriptTabWidget;
            }

            tabs.clear();
        }
    }
}