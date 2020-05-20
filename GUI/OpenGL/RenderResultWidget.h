//
// Created by binybrion on 5/13/20.
//

#ifndef VOXEL_L_SYSTEM_RENDERRESULTWIDGET_H
#define VOXEL_L_SYSTEM_RENDERRESULTWIDGET_H

#include <QtWidgets/QWidget>
#include <mat4x4.hpp>
#include "ProjectSaverLoader/UserDefinedInstances.h"

class QLineEdit;

namespace ModelLoading
{
    class Model;
}

namespace ProjectSaverLoader
{
    class ProjectDetails;
}

namespace Ui
{
    class RenderResultWidget;
}

namespace GUI
{
    namespace OpenGL
    {
        /**
         * Manages the render related functionality of a script by storing the resources used for rendering as well as
         * given the user the ability to interact with the rendered result.
         */

        class RenderResultWidget : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when there was an error loading a model during a render loop.
                 */
                void errorLoadingModel();

                /**
                * Emitted whenever the user performs some input causing the user-instance cubes to change in some way.
                 * Forwarded from GLWidget.
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
                explicit RenderResultWidget(QWidget *parent = nullptr);

                /**
                 * Forwards the call to GLWidget to render the results of a script interpretation.
                 */
                void addModelInstances();

                /**
                 * Adds an instance of the model identified by the passed in file to the render scene.
                 *
                 * The instance added will by default be located at the origin.
                 *
                 * Any previous user action history is removed.
                 *
                 * Request forwarded to GLWidget.
                 *
                 * @param modelFileName the model to add an instance of.
                 */
                void addUserRequestedModelInstances(const std::vector<::ProjectSaverLoader::UserDefinedInstances> &modelInstances);

                /**
                 * Enable or disable the button that allows the user to add an instance to the render. This is enabled
                 * if the user is viewing saved favourite result.
                 *
                 * @param enable true to enable the button
                 */
                void enableAddInstanceButton(bool enable);

            public slots:

                /**
                 * Forwards the request to GLWidget to upload the model to vRam, and adds the model to the list of models
                 * that can be added by the user as instances.
                 *
                 * @param model to render
                 */
                void uploadModelVRam(const ::ModelLoading::Model& model);

            private slots:

                /**
                 * Asks the user to specify a location to export to, and then forwards the request for exporting the
                 * project to the GLWidget.
                 */
                void handleRenderExport();

                /**
                 * Handles any change in the rotation X field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleRotationX();

                /**
                 * Handles any change in the rotation Y field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleRotationY();

                /**
                 * Handles any change in the rotation Z field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleRotationZ();

                /**
                 * Handles any change in the scale X field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleScaleX();

                /**
                 * Handles any change in the scale Y field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleScaleY();

                /**
                 * Handles any change in the scale Z field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleScaleZ();

                /**
                 * Handles any change in the translation X field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleTranslationX();

                /**
                 * Handles any change in the translation Y field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleTranslationY();

                /**
                 * Handles any change in the translation Z field, ensuring a valid input is given and if so, updates the
                 * transformation that will be applied to any selected instances.
                 */
                void handleTranslationZ();

            private:

                // Stores the result of trying to convert a string to a number
                struct ConvertNumberResult
                {
                    bool success;
                    float number;
                };

                /**
                 * Adds the appropriate widgets to the event filter.
                 */
                void applyEventFilter();

                /**
                 * Attempts to convert the text held in the given line edit to a valid number.
                 *
                 * @param field containing the text to convert.
                 * @return structure specifying the result of trying to convert the text
                 */
                ConvertNumberResult convertFieldNumber(const QString &text);

                /**
                 * Overrides the events for the radio buttons and the add instance button to ensure that if any of them are clicked,
                 * the focus goes to the GLWidget and not the aforementioned widgets themselves. This allows the user to click a radio button and
                 * apply the transformation changes without first having to click the GLWidget to get its focus.
                 *
                 * @param object that experienced an event
                 * @param event that the passed in object experienced
                 * @return false. All objects associated with this filter see the event targeted for them
                 */
                bool eventFilter(QObject *object, QEvent *event) override;

                /**
                 * Sets default transformation magnitudes to the appropriate line edits.
                 */
                void setDefaultTransformationValues();

                /**
                 * Sets up the connections used by this object's widgets.
                 */
                void setupConnections();

                Ui::RenderResultWidget *ui = nullptr;

                // The visual background to give to a label when it contains an invalid number.
                const QString errorStyleSheet = "background-color: rgba(128, 0, 0, 32);";
        };
    }
}

#endif //VOXEL_L_SYSTEM_RENDERRESULTWIDGET_H
