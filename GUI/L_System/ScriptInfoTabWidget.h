//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTINFOTABWIDGET_H
#define VOXEL_L_SYSTEM_SCRIPTINFOTABWIDGET_H

#include <QtWidgets/QTabWidget>

namespace ModelLoading
{
    class Model;
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
                 * Uploads the information stored in the various tabs that hold parameters for the script into the
                 * static ScriptInput data structure.
                 */
                void setScriptInput() const;

            private:

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                ConstantTabContent *constantTabContent = nullptr;
                RuleTabContent *ruleTabContent = nullptr;
                VariableTabContent *variableTabContent = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTINFOTABWIDGET_H
