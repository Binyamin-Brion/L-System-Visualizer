//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_VARIABLETABCONTENT_H
#define VOXEL_L_SYSTEM_VARIABLETABCONTENT_H

#include <QtWidgets/QWidget>

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
                 * Get the valid variable names. This forwards the result of calling the appropriate function in
                 * VariableWidget object.
                 *
                 * @return list of valid variable names
                 */
                std::vector<QString> getVariableNames() const;

            private slots:

                /**
                 * Prompts the user to select a file that contains a model to load, and attempts to load it.
                 */
                void loadNewModel();

            private:
                Ui::VariableTabContent *ui = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLETABCONTENT_H
