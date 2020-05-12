//
// Created by binybrion on 5/8/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTOUTPUTWIDGET_H
#define VOXEL_L_SYSTEM_SCRIPTOUTPUTWIDGET_H

#include <QtWidgets/QWidget>

class QVBoxLayout;

namespace L_System
{
    namespace Execution
    {
        class Token;
    }
}

namespace GUI
{
    namespace Output
    {
        namespace L_System
        {
            class ScriptOutputEntry;

            class ScriptOutputWidget : public QWidget
            {
                Q_OBJECT

                public:

                    /**
                     * Initializes the widget with the parent that has ownership over this object.
                     *
                     * @param parent that owns this object
                     */
                    explicit ScriptOutputWidget(QWidget *parent = nullptr);

                    /**
                     * Removes the visual output showing the result of the previous script execution.
                     */
                    void removePreviousResult();

                public slots:

                    /**
                     * Shows the output of the given execution result.
                     *
                     * @param tokens result of executing a script
                     */
                    void showSavedScriptOutput(const std::vector<std::vector<::L_System::Execution::Token>>& tokens);

                    /**
                     * Shows the output of executing the script.
                     */
                    void showScriptOutput();

                private:

                    /**
                     * Creates the entries to display the passed in execution result. This function is called by both
                     * showSavedScriptOutput() and showScriptOutput().
                     *
                     * @param tokens result of executing a script
                     */
                    void displayScriptOutput(const std::vector<std::vector<::L_System::Execution::Token>>& tokens);

                    QVBoxLayout *layout = nullptr;
                    std::vector<ScriptOutputEntry*> entries;
            };
        }
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTOUTPUTWIDGET_H
