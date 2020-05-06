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

            setupConnections();

            // These are the three operations allows on the stack for the transformations.
            ui->rotationStackComboBox->addItem("No Stack Action");
            ui->rotationStackComboBox->addItem("Push");
            ui->rotationStackComboBox->addItem("Pop");

            ui->translationStackComboBox->addItem("No Stack Action");
            ui->translationStackComboBox->addItem("Push");
            ui->translationStackComboBox->addItem("Pop");

            // By default, no transformations are done.
            disableRotation();
            disableTranslation();
        }

        void ConstantEntryInformation::handleRotationCheckBox(int state)
        {
            if(state == Qt::Checked)
            {
                enableRotation();
            }
            else
            {
                disableRotation();
            }
        }

        void ConstantEntryInformation::handleTranslationCheckBox(int state)
        {
            if(state == Qt::Checked)
            {
                enableTranslation();
            }
            else
            {
                disableTranslation();
            }
        }

        void ConstantEntryInformation::setupConnections()
        {
            connect(ui->rotationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleRotationCheckBox(int)));

            connect(ui->translationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleTranslationCheckBox(int)));
        }

        void ConstantEntryInformation::disableRotation()
        {
            // Display the item in the rotation stack operation combo box when the rotation part is disabled.
            ui->rotationStackComboBox->addItem(disabledOperation);
            ui->rotationStackComboBox->setCurrentIndex(disabledOperationIndex);

            ui->rotationStackComboBox->setEnabled(false);
            ui->rotationLineEditX->setEnabled(false);
            ui->rotationLineEditY->setEnabled(false);
            ui->rotationLineEditZ->setEnabled(false);
        }

        void ConstantEntryInformation::disableTranslation()
        {
            // Display the item in the translation stack operation combo box when the translation part is disabled.
            ui->translationStackComboBox->addItem(disabledOperation);
            ui->translationStackComboBox->setCurrentIndex(disabledOperationIndex);

            ui->translationStackComboBox->setEnabled(false);
            ui->translationLineEditX->setEnabled(false);
            ui->translationLineEditY->setEnabled(false);
            ui->translationLineEditZ->setEnabled(false);
        }

        void ConstantEntryInformation::enableRotation()
        {
            // Remove the item displayed in the stack operation box when this part of the script is disabled.
            ui->rotationStackComboBox->removeItem(disabledOperationIndex);

            ui->rotationStackComboBox->setCurrentIndex(firstActionIndex);

            ui->rotationStackComboBox->setEnabled(true);
            ui->rotationLineEditX->setEnabled(true);
            ui->rotationLineEditY->setEnabled(true);
            ui->rotationLineEditZ->setEnabled(true);
        }

        void ConstantEntryInformation::enableTranslation()
        {
            // Remove the item displayed in the stack operation box when this part of the script is disabled.
            ui->translationStackComboBox->removeItem(disabledOperationIndex);

            ui->translationStackComboBox->setCurrentIndex(firstActionIndex);

            ui->translationStackComboBox->setEnabled(true);
            ui->translationLineEditX->setEnabled(true);
            ui->translationLineEditY->setEnabled(true);
            ui->translationLineEditZ->setEnabled(true);
        }
    }
}