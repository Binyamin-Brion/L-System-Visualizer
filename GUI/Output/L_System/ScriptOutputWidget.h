//
// Created by binybrion on 5/8/20.
//

#ifndef VOXEL_L_SYSTEM_SCRIPTOUTPUTWIDGET_H
#define VOXEL_L_SYSTEM_SCRIPTOUTPUTWIDGET_H

#include <QtWidgets/QWidget>

class QVBoxLayout;

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

                public slots:

                    /**
                     * Shows the output of executing the script.
                     */
                    void showScriptOutput();

                private:

                    /**
                     * Removes the visual output showing the result of the previous script execution.
                     */
                    void removePreviousResult();

                    QVBoxLayout *layout = nullptr;
                    std::vector<ScriptOutputEntry*> entries;
            };
        }
    }
}

#endif //VOXEL_L_SYSTEM_SCRIPTOUTPUTWIDGET_H
