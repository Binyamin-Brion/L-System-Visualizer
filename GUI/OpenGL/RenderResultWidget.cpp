//
// Created by binybrion on 5/13/20.
//

#include <Render/DataStructures/TransformationIdentifier.h>
#include "RenderResultWidget.h"
#include "ui_renderResultWidget.h"

namespace GUI
{
    namespace OpenGL
    {
        RenderResultWidget::RenderResultWidget(QWidget *parent)
                            :
                                QWidget{parent},
                                ui{new Ui::RenderResultWidget}
        {
            ui->setupUi(this);

            applyEventFilter();

            setDefaultTransformationValues();

            ui->splitter_3->setStretchFactor(1, 1);

            // Call this before setting the a radio button as enabled so that by default there is a transformation to apply
            // to selected models.
            setupConnections();

            // By default make the X-Transformation active (no particular reason why).
            ui->translationXRadioButton->setChecked(true);

            handleTranslationX();
        }

        void RenderResultWidget::addModelInstances()
        {
            ui->openGLWidget->addModelInstances();
        }

        void RenderResultWidget::addUserRequestedModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &modelInstances)
        {
            ui->openGLWidget->addUserRequestedModelInstances(modelInstances);
        }

        void RenderResultWidget::enableAddInstanceButton(bool enable)
        {
            ui->addInstanceButton->setEnabled(enable);
        }

        // Beginning of public slots

        void RenderResultWidget::uploadModelVRam(const ::ModelLoading::Model &model)
        {
            ui->openGLWidget->uploadModelGPU(model);

            ui->modelComboBox->addItem(model.getModelFileName());
        }

        // Beginning of private slots

        // Note: all of the handleXXX functions have the same idea  as the rotationX, thus only that function is commented.

        void RenderResultWidget::handleRotationX()
        {
            // Note it is assumed that if the user is changing the X-rotation line edit then that is the next transformation
            // they want to apply, and so the radio button is set as enabled to indicate that.

            ui->rotationXRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->rotationXLineEdit->text());

            if(conversionResult.success)
            {
                ui->rotationXLineEdit->setStyleSheet(styleSheet()); // Restore default line edit look to to indicate success.

                // Have the transformation applied to selected instances be the transformation associated with this radio button with the given magnitude.
                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::RotationX, conversionResult.number});
            }
            else
            {
                // Show an error in the line edit as it contains an invalid number.
                ui->rotationXLineEdit->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleRotationY()
        {
            ui->rotationYRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->rotationYLineEdit->text());

            if(conversionResult.success)
            {
                ui->rotationYLineEdit->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::RotationY, conversionResult.number});
            }
            else
            {
                ui->rotationYLineEdit->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleRotationZ()
        {
            ui->rotationZRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->rotationZLineEdit->text());

            if(conversionResult.success)
            {
                ui->rotationZLineEdit->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::RotationZ, conversionResult.number});
            }
            else
            {
                ui->rotationZLineEdit->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleScaleX()
        {
            ui->scaleXRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->scaleXLineEdit->text());

            if(conversionResult.success)
            {
                ui->scaleXLineEdit->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::ScaleX, conversionResult.number});
            }
            else
            {
                ui->scaleXLineEdit->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleScaleY()
        {
            ui->scaleYRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->scaleYLineEdit->text());

            if(conversionResult.success)
            {
                ui->scaleYRadioButton->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::ScaleY, conversionResult.number});
            }
            else
            {
                ui->scaleYRadioButton->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleScaleZ()
        {
            ui->scaleZRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->scaleZRadioButton->text());

            if(conversionResult.success)
            {
                ui->scaleZRadioButton->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::ScaleZ, conversionResult.number});
            }
            else
            {
                ui->scaleZRadioButton->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleTranslationX()
        {
            ui->translationXRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->translationXLineEdit->text());

            if(conversionResult.success)
            {
                ui->translationXLineEdit->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::TranslationX, conversionResult.number});
            }
            else
            {
                ui->translationXLineEdit->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleTranslationY()
        {
            ui->translationYRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->translationYLineEdit->text());

            if(conversionResult.success)
            {
                ui->translationYLineEdit->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::TranslationY, conversionResult.number});
            }
            else
            {
                ui->translationYLineEdit->setStyleSheet(errorStyleSheet);
            }
        }

        void RenderResultWidget::handleTranslationZ()
        {
            ui->translationZRadioButton->setChecked(true);

            ConvertNumberResult conversionResult = convertFieldNumber(ui->translationZLineEdit->text());

            if(conversionResult.success)
            {
                ui->translationZLineEdit->setStyleSheet(styleSheet());

                ui->openGLWidget->setTransformationData({::Render::DataStructures::TransformationIdentifier::TranslationZ, conversionResult.number});
            }
            else
            {
                ui->translationZLineEdit->setStyleSheet(errorStyleSheet);
            }
        }

        // Beginning of private functions

        void RenderResultWidget::applyEventFilter()
        {
            // All radio buttons and the add instance button are associated with the specified event filter.
            ui->addInstanceButton->installEventFilter(this);

            ui->rotationXRadioButton->installEventFilter(this);
            ui->rotationYRadioButton->installEventFilter(this);
            ui->rotationZRadioButton->installEventFilter(this);

            ui->scaleXRadioButton->installEventFilter(this);
            ui->scaleYRadioButton->installEventFilter(this);
            ui->scaleZRadioButton->installEventFilter(this);

            ui->translationXRadioButton->installEventFilter(this);
            ui->translationYRadioButton->installEventFilter(this);
            ui->translationZRadioButton->installEventFilter(this);
        }

        RenderResultWidget::ConvertNumberResult RenderResultWidget::convertFieldNumber(const QString &text)
        {
            bool success;

            float number = text.toFloat(&success);

            return {success, number};
        }

        void RenderResultWidget::setDefaultTransformationValues()
        {
            // Some random transformation values to apply.

            ui->rotationXLineEdit->setText("90"); // Degrees
            ui->rotationYLineEdit->setText("90");
            ui->rotationZLineEdit->setText("90");

            ui->scaleXLineEdit->setText("1"); // Ratio
            ui->scaleYLineEdit->setText("1");
            ui->scaleZLineEdit->setText("1");

            ui->translationXLineEdit->setText("1");
            ui->translationYLineEdit->setText("1");
            ui->translationZLineEdit->setText("1");
        }

        void RenderResultWidget::setupConnections()
        {
            connect(ui->openGLWidget, &GLWidget::modelInstancesChanged, [this](const std::vector<::ProjectSaverLoader::UserDefinedInstances>&matrices) { emit modelInstancesChanged(matrices); });

            connect(ui->addInstanceButton, &QPushButton::clicked, [this]() { ui->openGLWidget->addUserRequestedModelInstance(ui->modelComboBox->currentText()); });

            connect(ui->undoButton, SIGNAL(clicked()), ui->openGLWidget, SLOT(undoUserAction()));


            connect(ui->rotationXLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleRotationX()));

            connect(ui->rotationYLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleRotationY()));

            connect(ui->rotationZLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleRotationZ()));

            connect(ui->scaleXLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleScaleX()));

            connect(ui->scaleYLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleScaleY()));

            connect(ui->scaleZLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleScaleZ()));

            connect(ui->translationXLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleTranslationX()));

            connect(ui->translationYLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleTranslationY()));

            connect(ui->translationZLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(handleTranslationZ()));


            connect(ui->rotationXRadioButton, SIGNAL(clicked()), this, SLOT(handleRotationX()));

            connect(ui->rotationYRadioButton, SIGNAL(clicked()), this, SLOT(handleRotationY()));

            connect(ui->rotationZRadioButton, SIGNAL(clicked()), this, SLOT(handleRotationZ()));

            connect(ui->scaleXRadioButton, SIGNAL(clicked()), this, SLOT(handleScaleX()));

            connect(ui->scaleYRadioButton, SIGNAL(clicked()), this, SLOT(handleScaleY()));

            connect(ui->scaleZRadioButton, SIGNAL(clicked()), this, SLOT(handleScaleZ()));

            connect(ui->translationXRadioButton, SIGNAL(clicked()), this, SLOT(handleTranslationX()));

            connect(ui->translationYRadioButton, SIGNAL(clicked()), this, SLOT(handleTranslationY()));

            connect(ui->translationZRadioButton, SIGNAL(clicked()), this, SLOT(handleTranslationZ()));
        }

        bool RenderResultWidget::eventFilter(QObject *object, QEvent *event)
        {
            if(event->type() == QEvent::FocusIn)
            {
                if(object == ui->addInstanceButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->rotationXRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->rotationYRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->rotationZRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->scaleXRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->scaleYRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->scaleZRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->translationXRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->translationYRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }

                if(object == ui->translationZRadioButton)
                {
                    ui->openGLWidget->setFocus();
                }
            }

            return false;
        }
    }
}