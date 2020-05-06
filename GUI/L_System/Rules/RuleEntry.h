//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULEENTRY_H
#define VOXEL_L_SYSTEM_RULEENTRY_H

#include <QtWidgets/QWidget>
#include <stack>

namespace Ui
{
    class RuleEntry;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * Represents a rule created that can be used in an L-Script.
         */

        class RuleEntry : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when rule is selected by checking its respective checkbox.
                 *
                 * @param newState state of the associated checkbox
                 */
                void ruleSelected(RuleEntry*, int newState);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit RuleEntry(QWidget *parent = nullptr);

                /**
                 * Adds the variable and constant names to the combox box that allow the user to specify the production
                 * for the rule. Only the variables are added to the combox that allow the user to select the starting
                 * production for the rule.
                 *
                 * @param variableNames list of valid variable names
                 * @param constantNames list of valid constants
                 */
                void updateAvailableRuleEntries(const std::vector<QString> &variableNames, const std::vector<QString> &constantNames);

            private slots:

                /**
                 * Determines whether to show the associated entry information based off of whether the associated checkbox
                 * is checked or not.
                 *
                 * @param state checked if the check box is checked
                 */
                void handleEntryVisibility(int state);

                /**
                 * Appends the chosen production the final production for the rule. The default production, stating to the
                 * user to click the combo box to select a production, is ignored if it is passed in.
                 *
                 * @param text to append to the rule's final production
                 */
                void handleProductionChosen(const QString &text);

                /**
                 * Undoes the most recent added production. If no such production exists, then no action is taken.
                 */
                void handleUndoButton();

            private:

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                const QString defaultRule = "Click to add production";

                Ui::RuleEntry *ui = nullptr;
                std::stack<int> addedProductions;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RULEENTRY_H