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
                 * Loads the project using the information stored in the ProjectDetails structure.
                 *
                 * @param scriptInformation structure holding the script information to display
                 */
                void loadProject(const ::ProjectSaverLoader::ScriptInformation &scriptInformation);

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
        };
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTINFOTABWIDGET_H
