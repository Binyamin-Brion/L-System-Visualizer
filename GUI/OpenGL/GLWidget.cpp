//
// Created by binybrion on 4/30/20.
//

#include "GLWidget.h"
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#include <QOpenGLDebugLogger>

namespace GUI
{
    namespace OpenGL
    {
        GLWidget::GLWidget(QWidget *parent)
                    :
                        QOpenGLWidget{parent}
        {
            setFocusPolicy(Qt::StrongFocus);
            setMouseTracking(true);

            // To ensure that the scene is constantly being rendered, a timer is used to make sure that the paintGL()
            // function is called every x amount of time.
            QTimer *timer = new QTimer;

            timer->setInterval(20);

            connect(timer, SIGNAL(timeout()), this, SLOT(update()));

            timer->start();

            // A debug surface is required in order to generate debug OpenGL messages.
            QSurfaceFormat surfaceFormat;
            surfaceFormat.setMajorVersion(4);
            surfaceFormat.setMinorVersion(4);
            surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
            surfaceFormat.setOption(QSurfaceFormat::DebugContext);

            setFormat(surfaceFormat);
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

                   // Check right now for an intersection, rather than waiting for an initial mouse movement.
                   commandCentre.checkRayIntersection(width(), height(), mouseX, mouseY);
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

                    // As soon as the shift key is released, the program is not in a state to display and check for
                    // intersections. As such, stop showing an intersections immediately.
                    commandCentre.resetIntersectionColours();
                    break;
            }
        }

        void GLWidget::initializeGL()
        {
            QOpenGLWidget::initializeGL();

            commandCentre.initialize();

            debugLogger = new QOpenGLDebugLogger{this};

            connect(debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(debugMessageGenerated(QOpenGLDebugMessage)), Qt::DirectConnection);

            if(!debugLogger->initialize())
            {
                throw std::runtime_error{"Unable to initialize the OpenGL Debug Logger!"};
            }

            debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging);
            debugLogger->enableMessages();

            glEnable(GL_DEPTH_TEST);
        }

        void GLWidget::mousePressEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::MiddleButton)
            {
                middleButtonDown = true;
            }
        }

        void GLWidget::mouseMoveEvent(QMouseEvent *event)
        {
            if(middleButtonDown)
            {
                commandCentre.getCamera().rotate(event->x(), event->y());
            }
            else if(keyShift_Pressed)
            {
                commandCentre.checkRayIntersection(width(), height(), event->x(), event->y());
            }

            // Keep track of most recent mouse position for when the shift key is pressed.
            mouseX = event->x();
            mouseY = event->y();
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

            commandCentre.render();
        }

        void GLWidget::resizeGL(int width, int height)
        {
            QOpenGLWidget::resizeGL(width, height);

            glViewport(0, 0, width, height);

            commandCentre.getCamera().updateScreenSize(width, height);
        }

        // Beginning of public slots

        void GLWidget::uploadModelGPU(const ::ModelLoading::Model &model)
        {
            commandCentre.addModel(model);
        }

        // Beginning of private slots

        void GLWidget::debugMessageGenerated(QOpenGLDebugMessage message) const
        {
            qDebug() << message;
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