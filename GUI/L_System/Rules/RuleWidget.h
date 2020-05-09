//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULEWIDGET_H
#define VOXEL_L_SYSTEM_RULEWIDGET_H

#include <QtWidgets/QWidget>
#include "RuleInformation.h"

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
                 * Get the information required to create Rule data structure for script execution for all of the rule entries.
                 *
                 * @return list of information required to create Rule data structures
                 */
                std::vector<RuleInformation> getRuleInformations() const;

                /**
                 * Makes the passed in names available to be used in a RuleEntry. See RuleEntry.h for more details.
                 *
                 * @param constantNames list of valid constants
                 */
                void updateAvailableConstants(std::vector<QString> constantNames);

                /**
                 * Makes the passed in names available to be used in a RuleEntry. See RuleEntry.h for more details.
                 *
                 * @param constantNames list of valid constants
                 */
                void updateAvailableVariables(std::vector<QString> variableNames);

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
