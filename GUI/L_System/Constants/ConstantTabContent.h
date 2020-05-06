//
// Created by binybrion on 5/3/20.
//

#ifndef VOXEL_L_SYSTEM_CONSTANTTABCONTENT_H
#define VOXEL_L_SYSTEM_CONSTANTTABCONTENT_H

#include <QtWidgets/QWidget>

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
            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ConstantTabContent(QWidget *parent = nullptr);

                /**
                 * Get the names of all of the entries that are valid.
                 *
                 * @return list of all valid entry names.
                 */
                std::vector<QString> getConstantNames() const;

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
