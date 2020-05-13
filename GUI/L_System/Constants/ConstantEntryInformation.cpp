//
// Created by binybrion on 5/3/20.
//

#include "ConstantEntryInformation.h"
#include "ui_constantEntryInformation.h"

namespace GUI
{
    namespace L_System
    {
        ConstantEntryInformation::ConstantEntryInformation(QWidget *parent)
                        :
                            QWidget{parent},
                            ui{new Ui::ConstantEntryInformation}
        {
            ui->setupUi(this);

            // These are the three operations allows on the stack for the transformations.
            ui->rotationStackComboBox->addItem(pushActionString);
            ui->rotationStackComboBox->addItem(popActionString);

            ui->translationStackComboBox->addItem(pushActionString);
            ui->translationStackComboBox->addItem(popActionString);

            // By default, a constant is assumed to be a translation, as they are easier to reason about.
            disableRotation();

            checkValidRotation();

            checkValidTranslation();

            setupConnections();
        }

        void ConstantEntryInformation::disableRotation()
        {
            ui->rotationStackComboBox->setEnabled(false);
            ui->angleLineEdit->setEnabled(false);
            ui->rotationLineEditX->setEnabled(false);
            ui->rotationLineEditY->setEnabled(false);
            ui->rotationLineEditZ->setEnabled(false);
        }

        void ConstantEntryInformation::disableTranslation()
        {
            ui->translationStackComboBox->setEnabled(false);
            ui->translationLineEditX->setEnabled(false);
            ui->translationLineEditY->setEnabled(false);
            ui->translationLineEditZ->setEnabled(false);
        }

        void ConstantEntryInformation::enableRotation()
        {
            ui->rotationStackComboBox->setEnabled(true);
            ui->angleLineEdit->setEnabled(true);
            ui->rotationLineEditX->setEnabled(true);
            ui->rotationLineEditY->setEnabled(true);
            ui->rotationLineEditZ->setEnabled(true);
        }

        void ConstantEntryInformation::enableTranslation()
        {
            ui->translationStackComboBox->setEnabled(true);
            ui->translationLineEditX->setEnabled(true);
            ui->translationLineEditY->setEnabled(true);
            ui->translationLineEditZ->setEnabled(true);
        }


        ::L_System::DataStructures::Rotation ConstantEntryInformation::getRotation() const
        {
            return ::L_System::DataStructures::Rotation{getRotationAngle(), getRotationAxis()};
        }

        ::L_System::DataStructures::StackOperation ConstantEntryInformation::getStackOperation() const
        {
            // Since the type of constant is stored in the EntryDeclaration (not in this class), a different way of finding
            // out if this constant information is for a Rotation or a Translation. Since it is known that the stack combo box
            // of the type this constant is not is NOT enabled, checking whether a stack operation combo box is enabled is
            // enough to check if this constant information pertains to a Rotation or a Translation.
            if(ui->rotationStackComboBox->isEnabled())
            {
                return determineStackOperation(ui->rotationStackComboBox->currentText());
            }
            else
            {
                return determineStackOperation(ui->translationStackComboBox->currentText());
            }
        }

        ::L_System::DataStructures::Translation ConstantEntryInformation::getTranslation() const
        {
            return ::L_System::DataStructures::Translation{getTranslationVector()};
        }

        bool ConstantEntryInformation::informationValid() const
        {
            return validInformation;
        }

        void ConstantEntryInformation::setInformation(const ::L_System::DataStructures::Constant &constant)
        {
            // If the constant is Rotation, set the correct index for stack operation for the rotations stack combo box
            // and initialize the rotation fields. Similar action is done if the constant is a Translation.
            if(constant.getRotation().isEnabled())
            {
                switch(constant.getStackOperation())
                {
                    case ::L_System::DataStructures::StackOperation::Pop:
                        ui->rotationStackComboBox->setCurrentIndex(0);
                        break;

                    case ::L_System::DataStructures::StackOperation::Push:
                        ui->rotationStackComboBox->setCurrentIndex(1);
                        break;
                }

                ui->angleLineEdit->setText(QString::number(constant.getRotation().getAngle()));

                ui->rotationLineEditX->setText(QString::number(constant.getTranslation().getTranslation().x));
                ui->rotationLineEditY->setText(QString::number(constant.getTranslation().getTranslation().y));
                ui->rotationLineEditZ->setText(QString::number(constant.getTranslation().getTranslation().z));
            }
            else if(constant.getTranslation().isEnabled())
            {
                switch(constant.getStackOperation())
                {
                    case ::L_System::DataStructures::StackOperation::Pop:
                        ui->translationStackComboBox->setCurrentIndex(0);
                        break;

                    case ::L_System::DataStructures::StackOperation::Push:
                        ui->translationStackComboBox->setCurrentIndex(1);
                        break;
                }

                ui->translationLineEditX->setText(QString::number(constant.getTranslation().getTranslation().x));
                ui->translationLineEditY->setText(QString::number(constant.getTranslation().getTranslation().y));
                ui->translationLineEditZ->setText(QString::number(constant.getTranslation().getTranslation().z));
            }
            else
            {
                // A constant can be only be a Rotation or a Translation. If it is neither, then an error occured.
                assert(false);
            }
        }

        // Beginning of private functions

        void ConstantEntryInformation::checkValidRotation()
        {
            const QString errorStyleSheet = "background-color: rgba(128, 0, 0, 32);";

            bool successRotation;

            bool successX;
            bool successY;
            bool successZ;

            ui->angleLineEdit->text().toDouble(&successRotation);
            ui->rotationLineEditX->text().toDouble(&successX);
            ui->rotationLineEditY->text().toDouble(&successY);
            ui->rotationLineEditZ->text().toDouble(&successZ);

            if(!successRotation)
            {
                ui->angleLineEdit->setStyleSheet(errorStyleSheet);
            }
            else
            {
                ui->angleLineEdit->setStyleSheet(styleSheet());
            }

            if(!successX)
            {
                ui->rotationLineEditX->setStyleSheet(errorStyleSheet);
            }
            else
            {
                ui->rotationLineEditX->setStyleSheet(styleSheet());
            }

            if(!successY)
            {
                ui->rotationLineEditY->setStyleSheet(errorStyleSheet);
            }
            else
            {
                ui->rotationLineEditY->setStyleSheet(styleSheet());
            }

            if(!successZ)
            {
                ui->rotationLineEditZ->setStyleSheet(errorStyleSheet);
            }
            else
            {
                ui->rotationLineEditZ->setStyleSheet(styleSheet());
            }

            validInformation = successRotation && successX && successY && successZ;

            emit informationChanged();
        }

        void ConstantEntryInformation::checkValidTranslation()
        {
            const QString errorStyleSheet = "background-color: rgba(128, 0, 0, 32);";

            bool successX;
            bool successY;
            bool successZ;

            ui->translationLineEditX->text().toDouble(&successX);
            ui->translationLineEditY->text().toDouble(&successY);
            ui->translationLineEditZ->text().toDouble(&successZ);

            if(!successX)
            {
                ui->translationLineEditX->setStyleSheet(errorStyleSheet);
            }
            else
            {
                ui->translationLineEditX->setStyleSheet(styleSheet());
            }

            if(!successY)
            {
                ui->translationLineEditY->setStyleSheet(errorStyleSheet);
            }
            else
            {
                ui->translationLineEditY->setStyleSheet(styleSheet());
            }

            if(!successZ)
            {
                ui->translationLineEditZ->setStyleSheet(errorStyleSheet);
            }
            else
            {
                ui->translationLineEditZ->setStyleSheet(styleSheet());
            }

            validInformation = successX && successY && successZ;

            emit informationChanged();
        }

        ::L_System::DataStructures::StackOperation ConstantEntryInformation::determineStackOperation(const QString &operationString) const
        {
            if(operationString == pushActionString)
            {
                return ::L_System::DataStructures::StackOperation::Push;
            }
            else
            {
                return ::L_System::DataStructures::StackOperation::Pop;
            }
        }

        float ConstantEntryInformation::getRotationAngle() const
        {
            return ui->angleLineEdit->text().toFloat();
        }

        glm::vec3 ConstantEntryInformation::getRotationAxis() const
        {
            return glm::vec3{ui->rotationLineEditX->text().toFloat(),
                             ui->rotationLineEditY->text().toFloat(),
                             ui->rotationLineEditZ->text().toFloat()};
        }

        glm::vec3 ConstantEntryInformation::getTranslationVector() const
        {
            return glm::vec3{ui->translationLineEditX->text().toFloat(),
                             ui->translationLineEditY->text().toFloat(),
                             ui->translationLineEditZ->text().toFloat()};
        }

        void ConstantEntryInformation::setupConnections()
        {
            connect(ui->translationLineEditX, &QLineEdit::textChanged, [this](const QString&) { checkValidTranslation(); });

            connect(ui->translationLineEditY, &QLineEdit::textChanged, [this](const QString&) { checkValidTranslation(); });

            connect(ui->translationLineEditZ, &QLineEdit::textChanged, [this](const QString&) { checkValidTranslation(); });

            connect(ui->angleLineEdit, &QLineEdit::textChanged, [this](const QString&) { checkValidRotation(); });

            connect(ui->rotationLineEditX, &QLineEdit::textChanged, [this](const QString&) { checkValidRotation(); });

            connect(ui->rotationLineEditY, &QLineEdit::textChanged, [this](const QString&) { checkValidRotation(); });

            connect(ui->rotationLineEditZ, &QLineEdit::textChanged, [this](const QString&) { checkValidRotation(); });
        }
    }
}