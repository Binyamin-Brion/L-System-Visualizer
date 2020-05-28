//
// Created by binybrion on 4/30/20.
//

#include "GLWidget.h"
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#include <QOpenGLDebugLogger>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QMessageBox>
#include <QTextStream>
#include <QDateTime>

namespace GUI
{
    namespace OpenGL
    {
        GLWidget::GLWidget(QWidget *parent)
                    :
                        QOpenGLWidget{parent},
                        openglErrorMessageFile{openGLErrorMessageFileLocation},
                        timer{new QTimer{this}}
        {
            // Try to open the OpenGL error message to write to; it's not fatal if it can't be opened, but troubleshooting the issue will be hard
            // if the error message that is displayed is not remembered.
            if(!openglErrorMessageFile.open(QIODevice::Append))
            {
                QMessageBox::warning(this, "Failed to Initialize OpenGL Error File", QString{"Could not open the file to which OpenGL errors are written to.\n\n"} +
                "While the program will work and OpenGL errors will be displayed, they will not be logged. \n\n File location: " + openGLErrorMessageFileLocation, QMessageBox::Ok);
            }

            QTextStream writeStream{&openglErrorMessageFile};

            writeStream << "\n\n" << QDateTime::currentDateTime().toString() << "\n\n";

            setFocusPolicy(Qt::StrongFocus);
            setMouseTracking(true);

            new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(undoUserAction()));

            // To ensure that the scene is constantly being rendered, a timer is used to make sure that the paintGL()
            // function is called every x amount of time.
            timer->setInterval(20);

            connect(timer, SIGNAL(timeout()), this, SLOT(update()));

            timer->start();

            // A debug surface is required in order to generate debug OpenGL messages.
            QSurfaceFormat surfaceFormat;
            surfaceFormat.setMajorVersion(4);
            surfaceFormat.setMinorVersion(2);
            surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
            surfaceFormat.setOption(QSurfaceFormat::DebugContext);

            setFormat(surfaceFormat);
        }

        void GLWidget::addModelInstances()
        {
            commandCentre.addModelInstances();
        }

        void GLWidget::addUserRequestedModelInstance(const QString &modelFileName)
        {
            commandCentre.addUserRequestedModelInstance(modelFileName);
        }

        void GLWidget::addUserRequestedModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &modelInstances)
        {
            commandCentre.addUserRequestedModelInstances(modelInstances);
        }

        void GLWidget::exportCurrentRender(const QString &exportLocation)
        {
            commandCentre.exportCurrentRender(exportLocation);
        }

        void GLWidget::keyPressEvent(QKeyEvent *event)
        {
           switch (event->key())
           {
               case Qt::Key_W:
                   keyW_Pressed = true;
                   break;

               case Qt::Key_A:
                   keyA_Pressed = true;
                   break;

               case Qt::Key_S:
                   keyS_Pressed = true;
                   break;

               case Qt::Key_D:
                   keyD_Pressed = true;
                   break;

               case Qt::Key_Shift:
                   keyShift_Pressed = true;
                   break;

               case Qt::Key_Z:
                   commandCentre.undoUserAction();
                   break;

                   // Whenever a user enters input that causes an instance of a model to (even potentially) change, update
                   // the instances of models held by the current favourite result.

               case Qt::Key_Left:

                   commandCentre.transformSelectedModels({transformationData.transformationIdentifier, -transformationData.amount});

                   emit modelInstancesChanged(commandCentre.getUserDefinedInstances());

                   break;

               case Qt::Key_Right:

                   commandCentre.transformSelectedModels(transformationData);

                   emit modelInstancesChanged(commandCentre.getUserDefinedInstances());

                   break;

               case Qt::Key_Delete:

                   commandCentre.deleteSelectedInstances();

                   emit modelInstancesChanged(commandCentre.getUserDefinedInstances());

                   break;

               case Qt::Key_Escape:
                   QApplication::quit();
           }
        }

        void GLWidget::keyReleaseEvent(QKeyEvent *event)
        {
            switch(event->key())
            {
                case Qt::Key_W:
                    keyW_Pressed = false;
                    break;

                case Qt::Key_A:
                    keyA_Pressed = false;
                    break;

                case Qt::Key_S:
                    keyS_Pressed = false;
                    break;

                case Qt::Key_D:
                    keyD_Pressed = false;
                    break;

                case Qt::Key_Shift:
                    keyShift_Pressed = false;
                    break;
            }
        }

        void GLWidget::initializeGL()
        {
            QOpenGLWidget::initializeGL();

            connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(cleanUp()));

            try
            {
                // If there is a error initializing OpenGL, that is a fatal flaw, so the user is told of the error
                // and the program terminates.
                commandCentre.initialize();
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::warning(this, "Error Initializing OpenGL", "The following error was encountered: \n\n" + QString{e.what()}
                        + "\n\nThe program will terminate.", QMessageBox::Ok);

                QApplication::quit();
            }

            debugLogger = new QOpenGLDebugLogger{this};

            connect(debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(debugMessageGenerated(QOpenGLDebugMessage)), Qt::DirectConnection);

            if(!debugLogger->initialize())
            {
                throw std::runtime_error{"Unable to initialize the OpenGL Debug Logger!"};
            }

            debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging);
            debugLogger->enableMessages();

            glEnable(GL_DEPTH_TEST);

            openGLVendor = std::string{reinterpret_cast<const char*>(glGetString(GL_VENDOR))};

            if(openGLVendor == "Intel")
            {
                QMessageBox::warning(this, "Intel GPU Warning", "On Intel GPUs, the program has been observed to not function correctly,"
                                                                "or even crash altogether.\n\nUnexpected behaviour may occur.", QMessageBox::Ok);
            }
        }

        void GLWidget::mousePressEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::MiddleButton)
            {
                middleButtonDown = true;
            }
            else if(event->button() == Qt::LeftButton)
            {
                commandCentre.checkRayIntersection(width(), height(), event->x(), event->y(), keyShift_Pressed);
            }
        }

        void GLWidget::mouseMoveEvent(QMouseEvent *event)
        {
            if(middleButtonDown)
            {
                commandCentre.getCamera().rotate(event->x(), event->y());
            }
        }

        void GLWidget::mouseReleaseEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::MiddleButton)
            {
                middleButtonDown = false;

                // This ensures that the next time the user tries to rotate the scene, there is no jump in what the camera sees.
                commandCentre.getCamera().resetFirstRotation();
            }
        }

        void GLWidget::paintGL()
        {
            QOpenGLWidget::paintGL();

            // Checking if a movement key is pressed every frame is removes any initial movement lag than if the camera
            // were updated as soon as the keyPressEvent function is called.
            checkMovementKeyPressed();

            // Due to internal structure of the render portion of the program, the actual upload of a model happens in the
            // render loop (only once- the first render loop after the model was requested to be loaded). This can result
            // in an error if the model was not loaded successfully.
            try
            {
                commandCentre.render();
            }
            catch(std::runtime_error &e)
            {
                // Stop the rendering process, otherwise the program may not respond well to user input as the message box is open.
                timer->stop();

                QMessageBox::critical(this, "Error With Rendering", "The following fatal error was generated: \n\n" + QString{e.what()} + "\n\nThis project will be saved and closed.", QMessageBox::Yes);

                // Start the chain reaction to create a new project; the state of the Render portion of this program is hard to know,
                // depending on what and how many models were being loaded. Thus it's easier to just terminate the current project.
                emit errorLoadingModel();
            }
        }

        void GLWidget::resizeGL(int width, int height)
        {
            QOpenGLWidget::resizeGL(width, height);

            glViewport(0, 0, width, height);

            commandCentre.getCamera().updateScreenSize(width, height);
        }

        void GLWidget::setTransformationData(const ::Render::DataStructures::TransformationData &transformationData)
        {
            this->transformationData = transformationData;
        }

        void GLWidget::wheelEvent(QWheelEvent *event)
        {
            // It is assumed that scrolling up is the same as the right-arrow key (positive transformation), while
            // a downwards scroll is a negative transformation.
            if(event->angleDelta().y() > 0)
            {
                commandCentre.transformSelectedModels(transformationData);

                emit modelInstancesChanged(commandCentre.getUserDefinedInstances());
            }
            else if(event->angleDelta().y() < 0)
            {
                commandCentre.transformSelectedModels({transformationData.transformationIdentifier, -transformationData.amount});

                emit modelInstancesChanged(commandCentre.getUserDefinedInstances());
            }
        }

        // Beginning of public slots

        void GLWidget::undoUserAction()
        {
            commandCentre.undoUserAction();
        }

        void GLWidget::uploadModelGPU(const ::ModelLoading::Model &model)
        {
            commandCentre.addModel(model);
        }

        // Beginning of private slots

        void GLWidget::cleanUp()
        {
            makeCurrent();

            commandCentre.deleteOpenGLResources();

            doneCurrent();
        }

        void GLWidget::debugMessageGenerated(QOpenGLDebugMessage message)
        {
            #ifdef QT_DEBUG

                // On the Intel GPU tested (HD 4600, Windows 10), a bunch of low-severity warnings are
                // given, which are distracting. Thus LowSeverity warnings from Intel are ignored.
                if(message.severity() == QOpenGLDebugMessage::LowSeverity && openGLVendor != "Intel") // Do not allow LowSeverity warnings
                {                                                                                     // to be printed on Intel GPUs.
                    qDebug() << message;
                }

            #else

                // Notify the user of any serious OpenGL errors. At this point the program may not work as expected.
                if(message.severity() == QOpenGLDebugMessage::HighSeverity || message.severity() == QOpenGLDebugMessage::MediumSeverity)
                {
                    QMessageBox::critical(this, "OpenGL Related Error", "The following OpenGL error was received: " + message.message() +
                            "\n\n The program may not work as expected. Take note of this error message for trouble shooting.", QMessageBox::Ok);
                }

            #endif

            if(openglErrorMessageFile.isOpen())
            {
                QTextStream writeStream{&openglErrorMessageFile};

                writeStream << "OpenGL Error: " << message.message() << "\n";
            }
        }

        // Beginning of private functions

        void GLWidget::checkMovementKeyPressed()
        {
            // Makes sense to check for intersection if the user is not moving.
            // Also reduces any noticeable lag as a result of rendering not being fast enough, as a low FPS is not
            // very noticeable if nothing is moving.
            if(keyShift_Pressed)
            {
                return;
            }

            // Move camera in appropriate direction.
            if(keyW_Pressed)
            {
                commandCentre.getCamera().move(::Render::Camera::CameraMovement::Forwards);
            }

            if(keyA_Pressed)
            {
                commandCentre.getCamera().move(::Render::Camera::CameraMovement::Left);
            }

            if(keyS_Pressed)
            {
                commandCentre.getCamera().move(::Render::Camera::CameraMovement::Backwards);
            }

            if(keyD_Pressed)
            {
                commandCentre.getCamera().move(::Render::Camera::CameraMovement::Right);
            }
        }
    }
}