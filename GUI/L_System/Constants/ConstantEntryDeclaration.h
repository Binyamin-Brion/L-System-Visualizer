//
// Created by binybrion on 5/4/20.
//

#ifndef VOXEL_L_SYSTEM_CONSTANTENTRYDECLARATION_H
#define VOXEL_L_SYSTEM_CONSTANTENTRYDECLARATION_H

#include <QtWidgets/QWidget>
#include "../../../L_System/DataStructures/Constants/Constant.h"

namespace Ui
{
    class ConstantEntryDeclaration;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * Stores the declaration of a constant entry, and holds the information section of the entry in a separate object.
         *
         * See ConstantEntryDeclaration.cpp for more details.
         */

        class ConstantEntryDeclaration : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when a constant is selected by clicking its respective checkbox (marked with to be deleted text).
                 *
                 * @param newState state of the associated checkbox
                 */
                void constantEntrySelected(ConstantEntryDeclaration*, int newState);

                /**
                 * Emitted when a new name for the entry is given.
                 *
                 * @param newName name specified by the user for the entry
                 */
                void nameChanged(ConstantEntryDeclaration*, const QString &newName);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ConstantEntryDeclaration(QWidget *parent = nullptr);

                /**
                 * Get the equivalent constant token using the information stored in this entry.
                 *
                 * @return constant token for use in the execution of the L-Script
                 */
                ::L_System::DataStructures::Constant getConstantToken() const;

                /**
                 * Update the line edit field where the entry name is given visually to reflect if the given name is
                 * valid or not.
                 *
                 * @param valid true if the name specified for the entry is valid
                 */
                void nameValid(bool valid);

            private slots:

                /**
                 * Determines whether to show the associated entry information based off of whether the checkbox, a part
                 * of the entry declaration, is checked or not.
                 *
                 * @param state checked if the check box is checked
                 */
                void handleEntryVisibility(int state);

            private:

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setUpConnections();

                Ui::ConstantEntryDeclaration *ui = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_CONSTANTENTRYDECLARATION_H
