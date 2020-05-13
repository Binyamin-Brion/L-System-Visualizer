//
// Created by binybrion on 5/3/20.
//

#ifndef VOXEL_L_SYSTEM_CONSTANTTABCONTENT_H
#define VOXEL_L_SYSTEM_CONSTANTTABCONTENT_H

#include <QtWidgets/QWidget>
#include "../../../L_System/DataStructures/Constants/Constant.h"

namespace Ui
{
    class ConstantTabContent;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * The container object for the contents of the Constant Tabs.
         *
         * Holds the scroll area to view entries as well as buttons to add and remove entries.
         */

        class ConstantTabContent : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when an entry has its name or information changed, resulting in the constant becoming
                 * either valid or invalid.
                 */
                void constantsChangedValidity(std::vector<QString>);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ConstantTabContent(QWidget *parent = nullptr);

                /**
                 * Get the list of Constant equivalent tokens of all of the constant entries.
                 *
                 * @return list of equivalent Constant tokens
                 */
                std::vector<::L_System::DataStructures::Constant> getConstantsTokens() const;

                /**
                 * Creates entries for each of the constant data structures held in the passed in vector.
                 * Forwards request to ConstantsWidget.
                 *
                 * @param constants vector of constant data structure from which to create entries
                 */
                void loadEntries(const std::vector<::L_System::DataStructures::Constant> &constants);

            public slots:

                /**
                 * Updates the variables names that are currently being used.
                 *
                 * Forwards argument to ConstantsWidget.
                 *
                 * @param constantNames vector of valid variable names
                 */
                void setVariableNames(const std::vector<QString> &variableNames);

            private:

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                Ui::ConstantTabContent *ui = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_CONSTANTTABCONTENT_H
