//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULEWIDGET_H
#define VOXEL_L_SYSTEM_RULEWIDGET_H

#include <QtWidgets/QWidget>

class QVBoxLayout;

namespace GUI
{
    namespace L_System
    {
        class RuleEntry;

        /**
         *  Holds all of the rule entries held in the Rules Tab.
         */

        class RuleWidget : public QWidget
        {
                Q_OBJECT

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit RuleWidget(QWidget *parent = nullptr);

                /**
                 * Makes the passed in names available to be used in a RuleEntry. See RuleEntry.h for more details.
                 *
                 * @param variableNames list of valid variable names
                 * @param constantNames list of valid constants
                 */
                void updateAvailableRuleEntries(const std::vector<QString> &variableNames, const std::vector<QString> &constantNames);

            public slots:

                /**
                 * Called when a new entry needs to be created. This is done by clicking the appropriate button in the
                 * this object.
                 */
                void addRuleEntry();

                /**
                 * Called when the button to delete all of the selected variable entries is clicked.
                 */
                void handleDeleteButtonPushed();

            private slots:

                /**
                 * Called when an entry's associated checkbox has its status changed.
                 *
                 * @param variableEntry whose associated checkbox was changed
                 * @param newState the current state of the checkbox
                 */
                void handleRuleEntrySelected(RuleEntry *ruleEntry, int newState);

            private:
                QVBoxLayout *layout = nullptr;

                std::vector<RuleEntry*> rules;
                std::vector<RuleEntry*> selectedRules;

                std::vector<QString> variableNames;
                std::vector<QString> constantNames;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RULEWIDGET_H