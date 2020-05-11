//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTINFOTABWIDGET_H
#define VOXEL_L_SYSTEM_SCRIPTINFOTABWIDGET_H

#include <QtWidgets/QTabWidget>
#include "ProjectSaverLoader/ScriptInformation.h"

namespace ModelLoading
{
    class Model;
}

namespace ProjectSaverLoader
{
    class ProjectDetails;
}

namespace GUI
{
    namespace L_System
    {
        class ConstantTabContent;
        class RuleTabContent;
        class VariableTabContent;

        /**
         * Holds the tabs containing the required information to run a L-Script.
         */

        class ScriptInfoTabWidget : public QTabWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when a model was successfully loaded from a file, and that model now needs to be stored in
                 * GPU memory. Forwarded from the VariablesWidget object.
                 *
                 * @param model
                 */
                void modelLoaded(const ::ModelLoading::Model &model);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ScriptInfoTabWidget(QWidget *parent = nullptr);

                /**
                 * Stores the result of an execution as a Favourite Result structure.
                 *
                 * @param saveResultName name of the result that is being saved
                 * @param executionResult execution result to associate with the passed in name
                 */
                void addFavouriteScript(const QString &saveResultName, const std::vector<std::vector<::L_System::Execution::Token>> &executionResult);

                /**
                 * Gets the favourite script that was stored with the given name.
                 *
                 * @param saveResultName the name of the saved result to return
                 * @return result with the same name as what was passed in
                 */
                const ::ProjectSaverLoader::FavouriteResult& getFavouriteScript(const QString &saveResultName) const;

                /**
                 * Loads the project using the information stored in the ProjectDetails structure.
                 *
                 * @param scriptInformation structure holding the script information to display
                 */
                void loadProject(const ::ProjectSaverLoader::ScriptInformation &scriptInformation);

                /**
                 * Removes the favourite script that was stored with the given name.
                 *
                 * @param saveResultName the name of the saved result to return
                 */
                void removeFavouriteScript(const QString &saveResultName);

                /**
                 * Stores the script information into the structure passed in.
                 *
                 * @param projectDetails structure to hold the script information
                 */
                void saveProject(::ProjectSaverLoader::ProjectDetails &projectDetails);

                /**
                 * Uploads the information stored in the various tabs that hold parameters for the script into the
                 * static ScriptInput data structure.
                 */
                void setScriptInput();

            private:

                /**
                 * Accumulates all of the script details stored in the various tabs into the internal variables stored
                 * in this object.
                 */
                void accumulateScriptInformation();

                /**
                 * Checks if a model has been loaded already.
                 *
                 * @param modelFileLocation model file to check if it has been registered as having been loaded already.
                 * @return true if the model file has been processed before (and therefore the model within uploaded to vRam)
                 */
                bool modelAlreadyLoaded(const QString &modelFileLocation) const;

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                ConstantTabContent *constantTabContent = nullptr;
                RuleTabContent *ruleTabContent = nullptr;
                VariableTabContent *variableTabContent = nullptr;

                std::vector<QString> loadedModels;

                ::L_System::DataStructures::Variable scriptAxiom;
                std::vector<::L_System::DataStructures::Constant> scriptConstants;
                std::vector<::L_System::DataStructures::Rule> scriptRules;
                std::vector<::L_System::DataStructures::Variable> scriptVariables;
                std::vector<::ProjectSaverLoader::FavouriteResult> favouriteResults;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTINFOTABWIDGET_H
