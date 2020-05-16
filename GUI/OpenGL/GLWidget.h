//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_GLWIDGET_H
#define VOXEL_L_SYSTEM_GLWIDGET_H

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLDebugMessage>
#include "../../Render/CommandCentre.h"
#include "Render/DataStructures/TransformationData.h"

class QOpenGLDebugLogger;

namespace ModelLoading
{
    class Model;
}

namespace ProjectSaverLoader
{
    class ProjectDetails;
}

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

            signals:

                /**
                 * Emitted whenever the user performs some input causing the user-instance cubes to change in some way.
                 *
                 * @param (vector of UserDefineInstances) the vector of all user defined instances being rendered.
                 */
                void modelInstancesChanged(const std::vector<::ProjectSaverLoader::UserDefinedInstances>&);

            public:

                /**
                 * Initializes the widget using the passed in parent.
                 *
                 * @param parent that has ownership of this widget
                 */
                explicit GLWidget(QWidget *parent = nullptr);

                /**
                 * Forwards the request to the CommandCentre to render the instances of the script interpretation.
                 *
                 * Request forwarded to CommandCentre.
                 */
                void addModelInstances();

                /**
                 * Adds an instance of the model identified by the passed in file to the render scene.
                 *
                 * The instance added will by default be located at the origin.
                 *
                 * Request forwarded to CommandCentre.
                 *
                 * @param modelFileName the model to add an instance of.
                 */
                void addUserRequestedModelInstance(const QString &modelFileName);

                /**
                 * Add user instanced models that were loaded from a file.
                 *
                 * Any previous user action history is removed.
                 *
                 * Request forwarded to CommandCentre.
                 *
                 * @param modelInstances the instance models to render
                 */
                void addUserRequestedModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &modelInstances);

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

                /**
                 * Store the transformation that should be applied to the given selected instances if the left or right
                 * arrow keys are pressed.
                 *
                 * @param transformationData to apply if the arrow keys are pressed
                 *
                 */
                void setTransformationData(const ::Render::DataStructures::TransformationData &transformationData);

            public slots:

                /**
                 * Undoes the most recent user action done in the render scene, if there is one.
                 *
                 * Request forwarded to CommandCentre.
                 */
                void undoUserAction();

                /**
                 * Uploads a model into GPU memory, allowing instances of the model to be rendered. Request is forwarded
                 * to the CommandCentre.
                 *
                 * @param model to be uploaded into vRam
                 */
                void uploadModelGPU(const ::ModelLoading::Model &model);

            private slots:

                /**
                 * Ensures that all OpenGL resources are freed.
                 */
                void cleanUp();

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

                ::Render::DataStructures::TransformationData transformationData;
        };
    }
}

#endif //VOXEL_L_SYSTEM_GLWIDGET_H
