//
// Created by binybrion on 4/30/20.
//

#include "GLWidget.h"
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

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
        }

        void GLWidget::keyPressEvent(QKeyEvent *event)
        {
           switch (event->key())
           {
               case Qt::Key_W:
                   commandCentre.getCamera().move(::Render::Camera::CameraMovement::Forwards);
                   break;

               case Qt::Key_A:
                   commandCentre.getCamera().move(::Render::Camera::CameraMovement::Left);
                   break;

               case Qt::Key_S:
                   commandCentre.getCamera().move(::Render::Camera::CameraMovement::Backwards);
                   break;

               case Qt::Key_D:
                   commandCentre.getCamera().move(::Render::Camera::CameraMovement::Right);
                   break;

               case Qt::Key_Escape:
                   QApplication::quit();
                   break;
           }
        }

        void GLWidget::initializeGL()
        {
            QOpenGLWidget::initializeGL();

            commandCentre.initialize();

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

            commandCentre.render();
        }

        void GLWidget::resizeGL(int width, int height)
        {
            QOpenGLWidget::resizeGL(width, height);

            glViewport(0, 0, width, height);

            commandCentre.getCamera().updateScreenSize(width, height);
        }
    }
}