//
// Created by binybrion on 5/6/20.
//

#ifndef VOXEL_L_SYSTEM_RULETABCONTENT_H
#define VOXEL_L_SYSTEM_RULETABCONTENT_H

#include <QtWidgets/QWidget>
#include "RuleInformation.h"
#include "L_System/DataStructures/Rules/Rule.h"

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

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit RuleTabContent(QWidget *parent = nullptr);

                /**
                 * Get the list of information required to create a Rule data structure for all of the rule entries.
                 *
                 * @return list of RuleInformations for all Rule entries
                 */
                std::vector<RuleInformation> getRuleInformations() const;

                /**
                 * Creates entries for each of the rule data structures held in the passed in vector.
                 * Forwards request to RuleWidget.
                 *
                 * @param constants vector of rule data structure from which to create entries
                 */
                void loadRules(const std::vector<::L_System::DataStructures::Rule> &rules);

            public slots:

                /**
                 * Makes the passed in names available to be used in a RuleEntry. See RuleEntry.h for more details.
                 *
                 * @param constantNames list of valid constants
                 */
                void updateAvailableConstantEntries(std::vector<QString> constantNames);

                /**
                 * Makes the passed in names available to be used in a RuleEntry. See RuleEntry.h for more details.
                 *
                 * @param constantNames list of valid constants
                 */
                void updateAvailableVariableEntries(std::vector<QString> variableNames);

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
