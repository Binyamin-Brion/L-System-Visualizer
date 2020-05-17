//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULEENTRY_H
#define VOXEL_L_SYSTEM_RULEENTRY_H

#include <QtWidgets/QWidget>
#include <stack>
#include "RuleInformation.h"
#include "L_System/DataStructures/Rules/Rule.h"

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
                 *  Emitted when the predecessor for a rule entry changes.
                 *
                 *  @param RuleEntry* the reference to the entry whose predecessor changed
                 */
                void predecessorChanged(RuleEntry*);

                /**
                 *  Emitted when the probability of a rule entry changes.
                 *
                 *  @param const QString& the predecessor of the rule entry whose probability changed
                 *  @param (first) int the old probability value
                 *  @oaram (second) int the new probability value
                 */
                void probabilityValueChanged(const QString&, int, int);

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
                 * Initializes the widget with the parent that has ownership over this object, and initializes the field
                 * of the widget with the information contained in the Rule data structure.
                 *
                 * @param rule containing information for the widget fields
                 * @param parent that owns this object
                 */
                explicit RuleEntry(const ::L_System::DataStructures::Rule &rule, QWidget *parent = nullptr);

                /**
                 * Checks if the successor tokens contain at least one of the deleted constant names.
                 *
                 * @param deletedConstantNames names of constants that were deleted
                 * @return true if at least one successor tokens has of the deleted names
                 */
                bool checkForDeletedConstants(const std::vector<QString> &deletedConstantNames) const;

                /**
                 * Checks if the predecessor token or successor tokens variables contains at least one of the deleted variable names.
                 *
                 * @param deletedConstantNames names of variables that were deleted
                 * @return true if at least one successor tokens or the predecessor token has of the deleted names
                 */
                bool checkForDeletedVariableUse(const std::vector<QString> &deletedVariableNames) const;

                /**
                 * Get the information about this rule to construct an equivalent Rule data structure for the script execution.
                 *
                 * @return information to construct a Rule data structure
                 */
                RuleInformation getRuleInformation() const;

                /**
                 * Sets the maximum probability value for an entry.
                 *
                 * @param value maximum probability entry for an entry
                 */
                void setMaximumProbability(int value);

                /**
                 * Sets the index of the predecessor in the appropriate combo box.
                 *
                 * @param index that the appropriate combo box should have
                 */
                void setPredecessorIndex(int index);

                /**
                 * Adds the variable and constant names to the combox box that allow the user to specify the production
                 * for the rule. Only the variables are added to the combox that allow the user to select the starting
                 * production for the rule.
                 *
                 * @param variableNames list of valid variable names
                 * @param constantNames list of valid constants
                 */
                 void updateAvailableRuleEntries(std::vector<QString> constantNames, std::vector<QString> variableNames);

            private slots:

                /**
                 * Determines whether to show the associated entry information based off of whether the associated checkbox
                 * is checked or not.
                 *
                 * @param state checked if the check box is checked
                 */
                void handleEntryVisibility(int state);

                /**
                 * Handle a new probability value being chosen by keeping track of the previous probability value.
                 *
                 * @param value the new probability value for the entry
                 */
                void handleNewProbabilityValue(int value);

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
                std::stack<int> addedProductions; // The size of each production added. For convenience only.
                std::vector<QString> addedProductionsText; // The actual text of the productions added.

                bool ignoreChosenProductions = false;

                // Keep track of the previous probability value for when the user changes the probability value
                int previousProbabilityValue;
        };
    }
}

#endif //VOXEL_L_SYSTEM_RULEENTRY_H
