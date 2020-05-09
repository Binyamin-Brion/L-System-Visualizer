//
// Created by binybrion on 4/30/20.
//

#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QtWidgets/QMessageBox>
#include "../L_System/ScriptInput.h"
#include "../L_System/Execution/Executor.h"
#include "../L_System/Interpretation/Interpreter.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent)
                :
                    QMainWindow{parent},
                    ui{new Ui::MainWindow}
    {
        setMinimumSize(1280, 720);

        ui->setupUi(this);

//        ui->spinBox->setMinimum(1);
//
//        ui->splitter_2->setStretchFactor(ui->splitter_2->sizes().size() - 1, 1);
//
//        ui->splitter_3->setStretchFactor(ui->splitter_3->sizes().size() - 1, 1);
//
//        ui->splitter_4->setStretchFactor(ui->splitter_4->sizes().size() - 1, 1);
//
//        ui->splitter_5->setStretchFactor(ui->splitter_5->sizes().size() - 1, 1);
//
//        ui->splitter_6->setStretchFactor(ui->splitter_6->sizes().size() - 1, 1);

        setupConnections();
    }

    void MainWindow::keyPressEvent(QKeyEvent *event)
    {
        switch(event->key())
        {
            case Qt::Key_Escape:
                QApplication::quit();
                break;
        }
    }

    // Beginning of private slots

    void MainWindow::runScript()
    {
        ui->tabWidget->setScriptInput();

        // There must be rules to run the script. This implicitly also checks that there are valid variables and (maybe) constants
        // defined as these must exist in order for a rule to be created. *Constants are not required to create a rule.
        if(::L_System::ScriptInput::getRules().empty())
        {
            QMessageBox::critical(this, "Error running script", "No rules were specified. The script will not be executed.", QMessageBox::Ok);

            return;
        }

        ::L_System::Execution::Executor::execute(ui->runDepthSpinBox->value());

        ::L_System::Interpretation::Interpreter::interpret();

        ui->scrollAreaWidgetContents->showScriptOutput();
    }

    // Beginning of private functions

    void MainWindow::setupConnections()
    {
        connect(ui->tabWidget, SIGNAL(modelLoaded(const ::ModelLoading::Model&)), ui->openGLWidget, SLOT(uploadModelGPU(const ::ModelLoading::Model&)));

        connect(ui->runScriptButton, SIGNAL(clicked()), this, SLOT(runScript()));
    }
}