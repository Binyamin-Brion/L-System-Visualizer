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

    namespace Interpretation
    {
        class UnderflowError;
        class UnbalancedError;
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
                     * Adds an error message to the current depth entry if there was an unbalanced error at that depth level.
                     *
                     * @param depthLevel the depth level for which to check for an unbalanced error
                     * @param scriptOutputEntry the entry to which to add an error message if there is an unbalanced error
                     * @param unbalancedErrors vector of unbalanced errors that occurred from the interpretation of the L-Script
                     */
                    void addUnbalancedErrorMessage(unsigned int depthLevel, ScriptOutputEntry *scriptOutputEntry, const std::vector<::L_System::Interpretation::UnbalancedError> &unbalancedErrors);

                    /**
                     * Adds an error message to the current depth entry if there was an underflow error at that depth level.
                     *
                     * @param depthLevel the depth level for which to check for an unbalanced error
                     * @param scriptOutputEntry the entry to which to add an error message if there is an unbalanced error
                     * @param underflowErrors vector of underflowErrors errors that occurred from the interpretation of the L-Script
                     */
                    void addUnderflowErrorMessage(unsigned int depthLevel, ScriptOutputEntry *scriptOutputEntry, const std::vector<::L_System::Interpretation::UnderflowError> &underflowErrors);

                    /**
                     * Creates the entries to display the passed in execution result. This function is called by both
                     * showSavedScriptOutput() and showScriptOutput().
                     *
                     * @param tokens result of executing a script
                     */
                    void displayScriptOutput(const std::vector<std::vector<::L_System::Execution::Token>>& tokens,
                                             const std::vector<::L_System::Interpretation::UnbalancedError> &unbalancedErrors,
                                             const std::vector<::L_System::Interpretation::UnderflowError> &underflowErrors);

                    QVBoxLayout *layout = nullptr;
                    std::vector<ScriptOutputEntry*> entries;
            };
        }
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTOUTPUTWIDGET_H
