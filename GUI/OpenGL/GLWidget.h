//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_GLWIDGET_H
#define VOXEL_L_SYSTEM_GLWIDGET_H

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLDebugMessage>
#include "../../Render/CommandCentre.h"

class QOpenGLDebugLogger;

namespace GUI
{
    namespace OpenGL
    {
        /**
         * Holds the widget that displays the rendering of the generated L-System.
         * Additionally provides the means to interact with the rendered mesh and its environment using a
         * keyboard and mouse.
         */

        class GLWidget : public QOpenGLWidget
        {
                Q_OBJECT

            public:

                /**
                 * Initializes the widget using the passed in parent.
                 *
                 * @param parent that has ownership of this widget
                 */
                explicit GLWidget(QWidget *parent = nullptr);

                /**
                 * Handles key press events when this widget is in focus.
                 *
                 * @param event containing information about they key press
                 */
                void keyPressEvent(QKeyEvent *event) override;

                /**
                 * Handles key release events when this widget is in focus.
                 *
                 * @param event containing information about they key release
                 */
                void keyReleaseEvent(QKeyEvent *event) override;

                /**
                 * Sets up the appropriate OpenGL state for rendering and initializing any required OpenGL structures
                 * required for rendering.
                 */
                void initializeGL() override;

                /**
                 * Handles mouse press events when this widget is in focus.
                 *
                 * @param event containing information about the mouse press
                 */
                void mousePressEvent(QMouseEvent *event) override;

                /**
                 * Handles mouse movements made over this widget.
                 *
                 * @param event containing information about the mouse movement
                 */
                void mouseMoveEvent(QMouseEvent *event) override;

                /**
                 * Handles mouse release events when this widget is in focus.
                 *
                 * @param event containing information about the mouse release
                 */
                void mouseReleaseEvent(QMouseEvent *event) override;

                /**
                 * Renders the required mesh(es) by calling the appropriate functions in Render-related components.
                 */
                void paintGL() override;

                /**
                 * Handles a change in the size of this widget, ensuring that the OpenGL viewport is updated when this happens.
                 *
                 * @param width of the widget after changing size
                 * @param height of the widget after changing size
                 */
                void resizeGL(int width, int height) override;

            private slots:

                /**
                 * Prints messages created by the OpenGL implementation.
                 *
                 * @param message to be printed to the console
                 */
                void debugMessageGenerated(QOpenGLDebugMessage message) const;

            private:

                /**
                 * Checks if the movement keys are pressed, and if they are move the camera in the respective direction.
                 */
                void checkMovementKeyPressed();

                ::Render::CommandCentre commandCentre;
                QOpenGLDebugLogger *debugLogger = nullptr;

                bool middleButtonDown = false;

                bool keyW_Pressed = false;
                bool keyA_Pressed = false;
                bool keyS_Pressed = false;
                bool keyD_Pressed = false;
                bool keyShift_Pressed = false;

                // Keep track of the mouse coordinates so that when the shift key is pressed, a check for intersection
                // with the rendered objects can be performed immediately, without waiting for an initial mouse movement.
                int mouseX = 0;
                int mouseY = 0;
        };
    }
}

#endif //VOXEL_L_SYSTEM_GLWIDGET_H
