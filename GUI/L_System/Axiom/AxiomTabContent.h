//
// Created by binybrion on 5/5/20.
//

#ifndef VOXEL_L_SYSTEM_AXIOMTABCONTENT_H
#define VOXEL_L_SYSTEM_AXIOMTABCONTENT_H

#include <QtWidgets/QWidget>

namespace Ui
{
    class AxiomTabContent;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * Holds the tab content for the ScriptInfoTabWidget, allowing the initial axiom for the script to be provided.
         */

        class AxiomTabContent : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when the the button an updated list of variable names need to be shown in this widget.
                 */
                void refreshButtonClicked();

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit AxiomTabContent(QWidget *parent = nullptr);

            public slots:

                /**
                 * Adds the specified variables to the combox box that allows user to select an initial axiom from the
                 * existing valid variables.
                 *
                 * @param variableNames list of variables that can be used for an axiom. Variable names must be valid.
                 */
                void addVariable(const std::vector<QString> &variableNames);

                /**
                 * Removes the specified variables from the combox box that allows user to select an intial axiom from the
                 * existing valid variables.
                 *
                 * @param variableNames list of variables that be used for an axiom. Variable names have been added to this
                 *                      object for an effect to occur.
                 */
                void removeVariables(const std::vector<QString> &variableNames);

            private:

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                Ui::AxiomTabContent *ui = nullptr;
                std::vector<QString> storedVariableNames;
        };
    }
}

#endif //VOXEL_L_SYSTEM_AXIOMTABCONTENT_H
