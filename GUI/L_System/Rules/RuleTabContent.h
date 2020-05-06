//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULETABCONTENT_H
#define VOXEL_L_SYSTEM_RULETABCONTENT_H

#include <QtWidgets/QWidget>

namespace Ui
{
    class RuleTabContent;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * The container object for the contents of the Rules Tabs.
         *
         * Holds the scroll area to view entries as well as buttons to add and remove entries.
         */

        class RuleTabContent : public QWidget
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
                explicit RuleTabContent(QWidget *parent = nullptr);

                /**
                 * Makes the passed in names available to be used in a RuleEntry. See RuleEntry.h for more details.
                 *
                 * @param variableNames list of valid variable names
                 * @param constantNames list of valid constants
                 */
                void updateAvailableRuleEntries(const std::vector<QString> &variableNames, const std::vector<QString> &constantNames);

            private:

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                Ui::RuleTabContent *ui = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RULETABCONTENT_H
