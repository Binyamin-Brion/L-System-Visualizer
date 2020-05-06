//
// Created by binybrion on 5/3/20.
//

#ifndef VOXEL_L_SYSTEM_CONSTANTENTRYINFORMATION_H
#define VOXEL_L_SYSTEM_CONSTANTENTRYINFORMATION_H

#include <QtWidgets/QWidget>

namespace Ui
{
    class ConstantEntryInformation;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * Stores the information of a constant entry- the transformations done to a script.
         *
         * See ConstantEntryDeclaration.cpp for more details.
         */

        class ConstantEntryInformation : public QWidget
        {
                Q_OBJECT

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ConstantEntryInformation(QWidget *parent = nullptr);

            private slots:

                /**
                 * Handles whether to make the rotation related fields accessible, based off of whether the constant
                 * is supposed to manipulate the rotation of the script.
                 *
                 * @param state of the checkbox
                 */
                void handleRotationCheckBox(int state);

                /**
                 * Handles whether to make the translation related fields accessible, based off of whether the constant
                 * is supposed to manipulate the translation of the script.
                 *
                 * @param state of the checkbox
                 */
                void handleTranslationCheckBox(int state);

            private:

                /**
                 * Disables the rotation-related fields. Until they are enabled, no modifications can be made.
                 * Disabled if the constant does not modify the rotation of the script.
                 */
                void disableRotation();

                /**
                 * Disables the translation-related fields. Until they are enabled, no modifications can be made.
                 * Disabled if the constant does not modify the translation of the script.
                 */
                void disableTranslation();

                /**
                 * Enables the rotation-related fields. Until they are disabled, modifications can be made.
                 * Enabled if the constant modify the rotation of the script.
                 */
                void enableRotation();

                /**
                 * Enables the translation-related fields. Until they are disabled, modifications can be made.
                 * Enabled if the constant modify the translation of the script.
                 */
                void enableTranslation();

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                Ui::ConstantEntryInformation *ui = nullptr;

                // Stores the index of the default stack operation after re-enabling a part of the constant,
                const int firstActionIndex = 0;

                // Holds the text to display in the stack operation when a part of the script is disabled,
                const QString disabledOperation = "Activate by clicking checkbox.";

                // Holds the index of the item to show in the stack operation combo box when a part of the script is disabled.
                const int disabledOperationIndex = 3;
        };
    }
}


#endif //VOXEL_L_SYSTEM_CONSTANTENTRYINFORMATION_H
