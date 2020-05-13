//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_VARIABLEENTRY_H
#define VOXEL_L_SYSTEM_VARIABLEENTRY_H

#include <QtWidgets/QWidget>
#include "../../../L_System/DataStructures/Variable/Variable.h"

namespace Ui
{
    class VariableEntry;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * Represents a variable created that can be used in an L-Script.
         */

        class VariableEntry : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when a new model is selected for the variable. This causes the list of valid variables to be updated,
                 * as a valid variable must include a valid associated model.
                 */
                void modelSelected();

                /**
                 * Emitted when a new name for the entry is given.
                 *
                 * @param newName name specified by the user for the entry
                 */
                void nameChanged(VariableEntry*, const QString &newName);

                /**
                 * Emitted when a variable is selected by clicking its respective checkbox.
                 *
                 * @param newState state of the associated checkbox
                 */
                void variableSelected(VariableEntry*, int newState);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit VariableEntry(QWidget *parent = nullptr);

                /**
                 * Initializes the widget with the parent that has ownership over this object, and initializes the field
                 * of the widget with the information contained in the Variable data structure.
                 *
                 * @param variable containing information for the widget fields
                 * @param parent that owns this object
                 */
                explicit VariableEntry(const ::L_System::DataStructures::Variable &variable, QWidget *parent = nullptr);

                /**
                 * Called when a model was loaded, and a variable can now represent that loaded model.
                 *
                 * @param modelName file location of the model that was loaded
                 */
                void addModelEntry(const QString &modelName);

                /**
                 * Get the stored associated model name for this variable.
                 *
                 * @return associated model name
                 */
                QString getAssociatedModelName() const;

                /**
                 * Get the text used to show that no associated model is selected.
                 *
                 * @return the text used to show no associated model
                 */
                const QString& getNoModelText() const;

                /**
                 * Get the equivalent Variable data structure for this variable for use in the script execution.
                 *
                 * @return equivalent Variable data structure
                 */
                ::L_System::DataStructures::Variable getVariableToken() const;

                /**
                 * Update the line edit field where the entry name is given visually to reflect if the given name is
                 * valid or not.
                 *
                 * @param valid true if the name specified for the entry is valid
                 */
                void nameValid(bool valid);

                /**
                 * Sets the index of the associated model in the appropriate combo box.
                 *
                 * @param index the associated model combo box should have
                 */
                void setAssociatedModelIndex(int index);

            private:

                /**
                  * Sets up the connections used by this object's widgets.
                  */
                void setupConnections();

                Ui::VariableEntry *ui = nullptr;

                const QString noModelText = "No model selected.";

                // Keep track of whether the noModelText has been deleted; this has to be done once as the noModelText
                // is automatically added during construction of this object, and is removed when a model name is added.
                bool noModelItemDeleted = false;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLEENTRY_H
