//
// Created by binybrion on 5/3/20.
//

#ifndef VOXEL_L_SYSTEM_CONSTANTENTRYINFORMATION_H
#define VOXEL_L_SYSTEM_CONSTANTENTRYINFORMATION_H

#include <QtWidgets/QWidget>
#include "../../../L_System/DataStructures/Constants/StackOperation.h"
#include "../../../L_System/DataStructures/Constants/Rotation.h"
#include "../../../L_System/DataStructures/Constants/Translation.h"
#include <vec3.hpp>
#include "L_System/DataStructures/Constants/Constant.h"

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

            signals:

                void informationChanged();

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit ConstantEntryInformation(QWidget *parent = nullptr);

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
                 * Get the rotation of this constant. Has no meaning unless the constant is specified to be a Rotation.
                 *
                 * @return Rotation data structure using the information stored in this class
                 */
                ::L_System::DataStructures::Rotation getRotation() const;

                /**
                 * Get the stack operation of this constant. Depending on whether the constant is a Rotation or a Translation,
                 * the result of this function will come from the respective combo box.
                 *
                 * @return stack operation of this constant
                 */
                ::L_System::DataStructures::StackOperation getStackOperation() const;

                /**
                 * Get the translation of this constant. Has no meaning unless the constant is specified to be a Translation.
                 *
                 * @return Translation data structure using the information stored in this class
                 */
                ::L_System::DataStructures::Translation getTranslation() const;

                bool informationValid() const;

                /**
                 * Puts the information stored in the constant data structure in the appropriate fields of this widget.
                 *
                 * @param constant data structure holding the information to display in this widget
                 */
                void setInformation(const ::L_System::DataStructures::Constant &constant);

            private:

                /**
                 * Checks whether the information stored in the rotation section of the constant is valid.
                 * If it is, the related text fields are shown as being without error and the status of the validity
                 * of this constant is updated.
                 *
                 * Should only be called if this constant represents a rotation.
                 */
                void checkValidRotation();

                /**
                 * Checks whether the information stored in the translation section of the constant is valid.
                 * If it is, the related text fields are shown as being without error and the status of the validity
                 * of this constant is updated.
                 *
                 * Should only be called if this constant represents a translation.
                 */
                void checkValidTranslation();

                /**
                 * Determines the stack operation to return based off of the value in a combo box representing stack operation.
                 *
                 * @param operationString stored in the stack operation combo box
                 * @return the equivalent StackOperation data structure
                 */
                ::L_System::DataStructures::StackOperation determineStackOperation(const QString &operationString) const;

                /**
                 * Get the number equivalent angle of the rotation. Only valid if the constant is a Rotation.
                 *
                 * @return angle of the rotation as a number
                 */
                float getRotationAngle() const;

                /**
                 * Get the rotation axis of the rotation as a vector. Only valid if the constant is a Rotation.
                 *
                 * @return vector form of the rotation axis
                 */
                glm::vec3 getRotationAxis() const;

                /**
                 * Get the translation as a vector. Only valid if the constant is a Translation.
                 *
                 * @return vector form of the translation
                 */
                glm::vec3 getTranslationVector() const;

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                Ui::ConstantEntryInformation *ui = nullptr;

                const QString pushActionString = "Push";
                const QString popActionString = "Pop";

                bool validInformation = false;
        };
    }
}


#endif //VOXEL_L_SYSTEM_CONSTANTENTRYINFORMATION_H
