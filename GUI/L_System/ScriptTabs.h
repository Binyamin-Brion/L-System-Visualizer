//
// Created by binybrion on 5/12/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTTABS_H
#define VOXEL_L_SYSTEM_SCRIPTTABS_H

#include <QtWidgets/QTabWidget>
#include "ProjectSaverLoader/ProjectLoader.h"
#include "ProjectSaverLoader/ProjectSaver.h"

namespace GUI
{
    namespace Dialogs
    {
        class NewNameDialog;
    }

    namespace L_System
    {
        class ScriptTabWidget;

        /**
         *  Holds all of the scripts in a Tab Widget, allowing the user to switch between open scripts.
         */

        class ScriptTabs : public QTabWidget
        {
                struct Tab
                {
                    QString name;
                    ScriptTabWidget *scriptTabWidget = nullptr;
                };

                Q_OBJECT

            signals:

                /**
                 * Emitted when the Render portion of the program experienced an error loading a model and the current
                 * project needs to be saved before creating a new project.
                 */
                void requestProjectSave();

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ScriptTabs(QWidget *parent = nullptr);

                /**
                 * Creates a default tab with the script name being "Default". If a tab with the name "Default" already
                 * exists, no action is taken.
                 */
                void addDefaultTab();

                /**
                 * Loads the project contained in the file passed in. All scripts will be loaded as separate tabs.
                 *
                 * @param modelFileLocation location of file containing the project to load
                 */
                void openProject(const QString &modelFileLocation);

                /**
                 * Saves all of the scripts to the passed in file. Any previous contents of that file, if any, will be overwritten.
                 *
                 * @param modelFileLocation location to save the project to.
                 */
                void saveProject(const QString &modelFileLocation);

            public slots:

                /**
                 *  Gives the user the option of creating a new script, and a tab to go along with that script, by
                 *  entering a unique name for the script.
                 */
                void addNewTab();

                /**
                * Discards the current tab and the associated script. The user is warning of this action before proceeding.
                */
                void removeCurrentTab();

                /**
                * Gives the user the option to rename a script upon choosing a valid new name.
                */
                void renameCurrentTab();

            private:

                /**
                 * Creates a script with the given name. This function does the actual work of creating a script (and tab),
                 * and is called by both addNewTab() and addDefaultTab().
                 *
                 * @param tabName name of the tab representing the name of the script.
                 * @param position the position to add the new tab to. A negative value is ignored.
                 */
                void addNewTab(const QString &tabName, int position = -1);

                /**
                 * Finds the tab with the given name.
                 *
                 * @param tabName the tab whose name should match
                 * @return iterator to the tab that matches the given name. If no such tab exists, the iterator points to
                 *                  the end of the Tabs vector
                 */
                std::vector<Tab>::const_iterator findTabName(const QString &tabName) const;

                /**
                 * Removes all of the existing tabs, and deletes the scripts. If the scripts were not saved, then they
                 * are lost permanently.
                 */
                void removeExistingTabs();

                const QString defaultTabName = "Default";

                std::vector<Tab> tabs;

                ::ProjectSaverLoader::ProjectLoader projectLoader;
                ::ProjectSaverLoader::ProjectSaver projectSaver;
                ::ProjectSaverLoader::ProjectDetails projectDetails;

                Dialogs::NewNameDialog *newTabNameDialog = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTTABS_H
