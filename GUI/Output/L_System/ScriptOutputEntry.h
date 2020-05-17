//
// Created by binybrion on 5/8/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTOUTPUTENTRY_H
#define VOXEL_L_SYSTEM_SCRIPTOUTPUTENTRY_H

#include <QtWidgets/QWidget>

namespace Ui
{
    class ScriptOutputEntry;
}

namespace GUI
{
    namespace Output
    {
        namespace L_System
        {
            /**
             * Represents a depth result from executing a script.
             */

            class ScriptOutputEntry : public QWidget
            {
                public:

                    /**
                      * Initializes the widget with the parent that has ownership over this object.
                      *
                      * @param parent that owns this object
                      */
                    explicit ScriptOutputEntry(QWidget *parent = nullptr);

                    /**
                     * Appends the output for the depth that this entry represents to the visual output.
                     *
                     * @param text to append to the visual output
                     */
                    void appendResultText(const QString &text);

                    /**
                     * Sets the depth that this entry represents.
                     *
                     * @param depthResult depth level being represented by this widget
                     */
                    void setDepthResult(unsigned int depthResult);

                    /**
                     * Shows the error message the dedicated area for the error message for a given depth level.
                     *
                     * @param errorMessage for the depth level result this entry represents and display
                     */
                    void setErrorMessage(const QString &errorMessage);

                    /**
                     * Shows or hides the error message associated with the output this entry displays depending on the input argument.
                     *
                     * @param visible true if the error message should be shown; false otherwise
                     */
                    void showError(bool visible);

                private:
                    Ui::ScriptOutputEntry *ui = nullptr;
            };
        }
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTOUTPUTENTRY_H
