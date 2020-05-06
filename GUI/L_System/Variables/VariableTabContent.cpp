//
// Created by binybrion on 5/2/20.
//

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <ModelLoading/Model.h>
#include "VariableTabContent.h"
#include "ui_variableTabContent.h"

namespace GUI
{
    namespace L_System
    {
        VariableTabContent::VariableTabContent(QWidget *parent)
                            :
                                QWidget{parent},
                                ui{new Ui::VariableTabContent}
        {
            ui->setupUi(this);

            connect(ui->loadModelButton, SIGNAL(clicked()), this, SLOT(loadNewModel()));

            connect(ui->addVariableButton, SIGNAL(clicked()), ui->declaredVariables, SLOT(addVariableEntry()));

            connect(ui->deleteVariableButtons, SIGNAL(clicked()), ui->declaredVariables, SLOT(handleDeleteButtonPushed()));
        }

        std::vector<QString> VariableTabContent::getVariableNames() const
        {
            return ui->declaredVariables->getEntryNames();
        }

        void VariableTabContent::loadNewModel()
        {
            // Note: for some reason limiting the files that can be chosen in the file dialog based off of the file extension
            // does note work.
           QString modelFileLocation = QFileDialog::getOpenFileName(this, "Open Mode", QDir::homePath());

           if(modelFileLocation.isEmpty())
           {
               return;
           }

           if(!modelFileLocation.contains(".obj")) // The selected file may not actually be file containing a valid model.
           {
               int reply = QMessageBox::question(this, "Load Model", "The file you selected does not appear to an OBJ file.\n\n"
                                                         "Proceed to load the selected model?", QMessageBox::Yes | QMessageBox::No);

               if(reply == QMessageBox::No)
               {
                   return;
               }
           }

           // If the model was successfully loaded, then emit respective signal so that the model can be uploaded into
           // GPU memory.

           try
           {
               ::ModelLoading::Model loadedModel{modelFileLocation.toStdString()};

                emit modelLoaded(loadedModel);

                ui->declaredVariables->addModelEntry(modelFileLocation);
           }
           catch(std::runtime_error &e)
           {
               QMessageBox::critical(this, "Load Model", "Unable to load the requested model.\n\n" +
                                                         QString{e.what()}, QMessageBox::Ok);
           }
        }
    }
}
