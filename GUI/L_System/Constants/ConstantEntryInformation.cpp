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
            ui->rotationStackComboBox->addItem("Push");
            ui->rotationStackComboBox->addItem("Pop");

            ui->translationStackComboBox->addItem("Push");
            ui->translationStackComboBox->addItem("Pop");

            // By default, no rotation transformations are done.
            disableRotation();
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
            ui->rotationStackComboBox->setCurrentIndex(firstActionIndex);

            ui->rotationStackComboBox->setEnabled(true);
            ui->angleLineEdit->setEnabled(true);
            ui->rotationLineEditX->setEnabled(true);
            ui->rotationLineEditY->setEnabled(true);
            ui->rotationLineEditZ->setEnabled(true);
        }

        void ConstantEntryInformation::enableTranslation()
        {
            ui->translationStackComboBox->setCurrentIndex(firstActionIndex);

            ui->translationStackComboBox->setEnabled(true);
            ui->translationLineEditX->setEnabled(true);
            ui->translationLineEditY->setEnabled(true);
            ui->translationLineEditZ->setEnabled(true);
        }
    }
}