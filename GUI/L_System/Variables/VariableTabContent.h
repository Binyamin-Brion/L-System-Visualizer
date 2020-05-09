//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_VARIABLETABCONTENT_H
#define VOXEL_L_SYSTEM_VARIABLETABCONTENT_H

#include <QtWidgets/QWidget>
#include "../../../L_System/DataStructures/Variable/Variable.h"

namespace ModelLoading
{
    class Model;
}

namespace Ui
{
    class VariableTabContent;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * The container object for the contents of the Variables Tabs.
         *
         * Holds the scroll area to view entries as well as buttons to add and remove entries.
         */

        class VariableTabContent : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when an entry name changes, and the list of variables that can be used in a rule has to be updated.
                 */
                void entryNamesChanged(std::vector<QString>);

                /**
                 * Emitted when a model selected by the user from the file disk was successfully loaded.
                 *
                 * @param model the model object that was loaded
                 */
                void modelLoaded(const ::ModelLoading::Model &model);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit VariableTabContent(QWidget *parent = nullptr);

                /**
                 * Get the Variable data structure equivalent of the variable chosen to be the axiom for the script.
                 *
                 * @return Variable data structure equivalent for the axiom
                 */
                ::L_System::DataStructures::Variable getAxiom() const;

                /**
                 * Get the valid variable names. This forwards the result of calling the appropriate function in
                 * VariableWidget object.
                 *
                 * @return list of valid variable names
                 */
                std::vector<QString> getVariableNames() const;

                /**
                 * Get the list of equivalent Variable data structures for the script execution for all of the variable entries.
                 *
                 * @return list of equivalent Variable data structures
                 */
                std::vector<::L_System::DataStructures::Variable> getVariablesTokens() const;

            private slots:

                /**
                 * Updates the list of variables that can be chosen an axiom.
                 *
                 * @param optionList list of variable names that can be used as an axiom
                 */
                void refreshAxiomList(std::vector<QString> optionList);

                /**
                 * Prompts the user to select a file that contains a model to load, and attempts to load it.
                 */
                void loadNewModel();

            private:
                void setupConnections();

                Ui::VariableTabContent *ui = nullptr;
                QString invaliAxiomString = "No variables declared.";
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLETABCONTENT_H
